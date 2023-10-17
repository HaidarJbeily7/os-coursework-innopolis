#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define AGENT_PID_FILE "/var/run/agent.pid"
#define TEXT_FILE "./text.txt"

int agent_pid;

void handle_sigusr1(int signum) {
    char buffer[256];
    FILE *file = fopen(TEXT_FILE, "r");
    if (file == NULL) {
        perror("Error reading text file");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

void handle_sigusr2(int signum) {
    printf("Process terminating...\n");
    unlink(AGENT_PID_FILE);
    exit(0);
}

int main() {
    // Create and write agent's PID to the file
    agent_pid = getpid();
    int pid_file = open(AGENT_PID_FILE, O_CREAT | O_WRONLY, 0644);
    if (pid_file == -1) {
        perror("Error creating PID file");
        exit(1);
    }
    dprintf(pid_file, "%d", agent_pid);
    close(pid_file);

    // Set up signal handlers
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    // Read and print the contents of text.txt
    handle_sigusr1(SIGUSR1);

    while (1) {
        // Sleep indefinitely
        sleep(3600);
    }

    return 0;
}