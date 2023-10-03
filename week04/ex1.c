#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    pid_t child1, child2;
    
    child1 = fork();
    clock_t start_time = clock();
    if (child1 == 0)
    {
        clock_t start_time = clock();
        printf("Child 1: PID = %d, Parent PID = %d\n", getpid(), getppid());   
        sleep(2);

        clock_t end_time = clock();
        double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
        printf("Child 1 Execution Time: %.2f ms\n", execution_time);
    }
    else
    {
        
        child2 = fork();

        if (child2 == 0)
        {

            clock_t start_time = clock();
            printf("Child 2: PID = %d, Parent PID = %d\n", getpid(), getppid());

    
            sleep(4);    
            clock_t end_time = clock();
            double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
            printf("Child 2 Execution Time: %.2f ms\n", execution_time);
        }
        else
        {
    
            printf("Main Process: PID = %d\n", getpid());

    
            wait(NULL);
            wait(NULL);

            clock_t end_time = clock();
            double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
            printf("Main Process Execution Time: %.2f ms\n", execution_time);
        }
    }

    return 0;
}
