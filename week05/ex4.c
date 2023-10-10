#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// Mutexes for synchronization
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t k_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c_lock = PTHREAD_MUTEX_INITIALIZER;

// Do not modify these variables directly, use the functions below
int k = 0;
int c = 0;
int n = 0; // Input from command line

// Primality test (from ex3)
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

// Get next prime candidate
int get_number_to_check()
{
    int ret;
    pthread_mutex_lock(&k_lock);
    ret = k;
    if (k != n)
        k++;
    pthread_mutex_unlock(&k_lock);
    return ret;
}

// Increase prime counter
void increment_primes()
{
    pthread_mutex_lock(&c_lock);
    c++;
    pthread_mutex_unlock(&c_lock);
}

// Start routine for threads
void *check_primes(void *arg)
{
    while (1)
    {
        int num = get_number_to_check();
        if (num >= n)
            break; // All numbers checked
        if (is_prime(num))
        {
            increment_primes();
        }
    }
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
    int m = atoi(argv[2]);

    if (n <= 0 || m <= 0)
    {
        fprintf(stderr, "Both n and m must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[m];

    // Create and run threads
    for (int i = 0; i < m; i++)
    {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < m; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Number of primes in [0, %d) with %d threads: %d\n", n, m, c);

    return 0;
}
