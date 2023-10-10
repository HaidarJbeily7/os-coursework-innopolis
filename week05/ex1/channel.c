#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1024

int main()
{
    int pipefd[2];
    pid_t child_pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child_pid = fork(); // Create a child process

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        // This is the subscriber process
        close(pipefd[1]); // Close the write end of the pipe

        char message[MAX_MESSAGE_SIZE];
        ssize_t nbytes;

        while ((nbytes = read(pipefd[0], message, MAX_MESSAGE_SIZE)) > 0)
        {
            write(STDOUT_FILENO, message, nbytes); // Print the message to stdout
        }

        close(pipefd[0]);
    }
    else
    {
        // This is the publisher process
        close(pipefd[0]); // Close the read end of the pipe

        char message[MAX_MESSAGE_SIZE];
        ssize_t nbytes;

        while (1)
        {
            nbytes = read(STDIN_FILENO, message, MAX_MESSAGE_SIZE);
            if (nbytes <= 0)
                break;                         // End of input
            write(pipefd[1], message, nbytes); // Send message to subscriber
        }

        close(pipefd[1]);
    }

    return 0;
}
