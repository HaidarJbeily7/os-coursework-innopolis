#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    while (1)
    {
        printf("SimpleShell> ");
        char command[100];
        fgets(command, sizeof(command), stdin);

        command[strlen(command) - 1] = '\0';

        char *args[20]; // Array to hold command and arguments
        int i = 0;

        
        char *token = strtok(command, " ");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the argument array

        pid_t pid = fork();

        if (pid == 0)
        {
            if (i > 1 && strcmp(args[i - 1], "&") == 0)
            {
                args[i - 1] = NULL; 
                if (execvp(args[0], args) == -1)
                {
                    perror("execvp");
                }
            }
            else
            {
                // Run the command in the foreground
                if (execvp(args[0], args) == -1)
                {
                    perror("execvp");
                }
            }

            exit(0);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
        }
        else
        {
            // Parent process
            // If the command is not run in the background, wait for the child to finish
            if (i > 1 && strcmp(args[i - 1], "&") != 0)
            {
                wait(NULL);
            }
        }
    }

    return 0;
}
