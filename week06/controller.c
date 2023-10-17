#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define AGENT_PID_FILE "/var/run/agent.pid"

int agent_pid;

void handle_ctrlc(int signum) {
    // Send SIGTERM to the agent
    if (agent_pid > 0) {
        kill(agent_pid, SIGTERM);
    }
    exit(0);
}

int main() {
    // Check for a running agent by fetching its PID
    int pid_file = open(AGENT_PID_FILE, O_RDONLY);
    if (pid_file == -1) {
        perror("Error opening PID file");
        printf("Error: No agent found.\n");
        exit(1);
    }
    char pid_str[32];
    int n = read(pid_file, pid_str, sizeof(pid_str));
    if (n <= 0) {
        perror("No agent found");
        exit(1);
    }
    pid_str[n] = '\0';
    agent_pid = atoi(pid_str);
    close(pid_file);

    // Set up signal handler for Ctrl+C
    signal(SIGINT, handle_ctrlc);

    printf("Agent found.\n");

    while (1) {
        char command[10];
        printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent: ");
        scanf("%s", command);

        if (strcmp(command, "exit") == 0) {
            // Send SIGUSR2 and exit
            if (agent_pid > 0) {
                kill(agent_pid, SIGUSR2);
            }
            exit(0);
        } else if (strcmp(command, "read") == 0) {
            // Send SIGUSR1
            if (agent_pid > 0) {
                kill(agent_pid, SIGUSR1);
            }
        } else if (strcmp(command, "stop") == 0) {
            // Send SIGSTOP
            if (agent_pid > 0) {
                kill(agent_pid, SIGSTOP);
            }
        } else if (strcmp(command, "continue") == 0) {
            // Send SIGCONT
            if (agent_pid > 0) {
                kill(agent_pid, SIGCONT);
            }
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}