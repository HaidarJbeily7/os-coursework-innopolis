#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VECTOR_SIZE 120

// Function to calculate the dot product of two arrays
int dotProduct(int u[], int v[], int start, int end)
{
    int result = 0;
    for (int i = start; i < end; i++)
    {
        result += u[i] * v[i];
    }
    return result;
}

int main()
{
    int n, i;
    printf("Enter the number of processes (n): ");
    scanf("%d", &n);

    if (n % 2 != 0)
    {
        printf("n must be 2k->  k = {1,2,3,4,5...}.");
        return 1;
    }

    int u[VECTOR_SIZE], v[VECTOR_SIZE];

    
    srand(time(NULL));

    
    for (i = 0; i < VECTOR_SIZE; i++)
    {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    FILE *file = fopen("temp.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    pid_t pid;
    int segment_size = VECTOR_SIZE / n;
    int start = 0, end = segment_size;
    int child_result;

    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            // Child process
            child_result = dotProduct(u, v, start, end);

            // Write the result to the file
            fprintf(file, "%d\n", child_result);

            // Close the file and exit
            fclose(file);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
            return 1;
        }

        start = end;
        end = start + segment_size;
    }

    // The main process waits for all children to complete
    for (i = 0; i < n; i++)
    {
        wait(NULL);
    }

    fclose(file);

    
    file = fopen("temp.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int final_result = 0;
    int temp_result;

    for (i = 0; i < n; i++)
    {
        fscanf(file, "%d", &temp_result);
        final_result += temp_result;
    }

    
    fclose(file);

    printf("Dot Product: %d\n", final_result);

    return 0;
}
