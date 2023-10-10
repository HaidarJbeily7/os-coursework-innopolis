#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

// Global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread synchronization
int n;                                             // Range upper limit
int m;                                             // Number of threads
int *thread_results;                               // Array to store thread results

// primality test
bool is_prime(int num)
{
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

// Thread function to count primes in a subinterval
void *prime_counter(void *arg)
{
    int thread_id = *((int *)arg);
    int subinterval_size = n / m;
    int start = thread_id * subinterval_size;
    int end = (thread_id == m - 1) ? n : (start + subinterval_size);

    int count = 0;
    for (int i = start; i < end; i++)
    {
        if (is_prime(i))
            count++;
    }

    // Store the result in the global array
    pthread_mutex_lock(&mutex);
    thread_results[thread_id] = count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    if (n <= 0 || m <= 0)
    {
        fprintf(stderr, "Both n and m must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[m];
    int thread_ids[m];
    thread_results = (int *)malloc(m * sizeof(int));

    // Create and run threads
    for (int i = 0; i < m; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, prime_counter, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < m; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Calculate the total count of primes
    int total_count = 0;
    for (int i = 0; i < m; i++)
    {
        total_count += thread_results[i];
    }

    printf("Number of primes in [0, %d) with %d threads: %d\n", n, m, total_count);

    // Clean up
    free(thread_results);

    return 0;
}
