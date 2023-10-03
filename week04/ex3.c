#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            
            printf("Child %d: PID = %d, Parent PID = %d\n", i + 1, getpid(), getppid());
            sleep(5);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
            return 1;
        }
    }


    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }

    return 0;
}
