#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 256

struct Thread
{
    pthread_t id;
    int i;
    char message[MAX_MESSAGE_SIZE];
};

void *thread_function(void *arg)
{
    struct Thread *thread_data = (struct Thread *)arg;

    printf("Thread %d prints: %s\n", thread_data->i, thread_data->message);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0)
    {
        fprintf(stderr, "Number of threads must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    // Create an array of Thread structs
    struct Thread thread_data[num_threads];

    // Create and launch the threads
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].i = i;
        snprintf(thread_data[i].message, MAX_MESSAGE_SIZE, "Hello from thread %d", thread_data[i].i);

        // Create the thread
        if (pthread_create(&(thread_data[i].id), NULL, thread_function, &(thread_data[i])))
        {
            fprintf(stderr, "Error creating thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }

        // Inform about thread creation
        printf("Thread %d is created\n", thread_data[i].i);
        wait(NULL);
        pthread_join(thread_data[i].id, NULL);
    }

    return 0;
}
