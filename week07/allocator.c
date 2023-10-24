#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MEMORY_SIZE 10000000

unsigned int memory[MEMORY_SIZE];

// Initialize memory cells to zeros
void initialize_memory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }
}

bool allocate_first_fit(unsigned int adrs, unsigned int size)
{
    int i = 0;
    int freeCount = 0;
    int start = -1;

    while (i < MEMORY_SIZE)
    {
        if (memory[i] == 0)
        {
            if (start == -1)
            {
                start = i;
            }
            freeCount++;
        }
        else
        {
            if (freeCount >= size)
            {
                for (int j = start; j < start + size; j++)
                {
                    memory[j] = adrs;
                }
                return true;
            }
            start = -1;
            freeCount = 0;
        }
        i++;
    }

    return false; // Allocation failed
}

bool allocate_best_fit(unsigned int adrs, unsigned int size)
{
    int i = 0;
    int bestStart = -1;
    int bestSize = INT_MAX;
    int freeCount = 0;
    int currentStart = -1;

    while (i < MEMORY_SIZE)
    {
        if (memory[i] == 0)
        {
            if (currentStart == -1)
            {
                currentStart = i;
            }
            freeCount++;
        }
        else
        {
            if (freeCount >= size && freeCount < bestSize)
            {
                bestStart = currentStart;
                bestSize = freeCount;
            }
            currentStart = -1;
            freeCount = 0;
        }
        i++;
    }

    if (bestStart != -1)
    {
        for (int j = bestStart; j < bestStart + size; j++)
        {
            memory[j] = adrs;
        }
        return true;
    }

    return false; // Allocation failed
}

bool allocate_worst_fit(unsigned int adrs, unsigned int size)
{
    int i = 0;
    int worstStart = -1;
    int worstSize = -1;
    int freeCount = 0;
    int currentStart = -1;

    while (i < MEMORY_SIZE)
    {
        if (memory[i] == 0)
        {
            if (currentStart == -1)
            {
                currentStart = i;
            }
            freeCount++;
        }
        else
        {
            if (freeCount >= size && freeCount > worstSize)
            {
                worstStart = currentStart;
                worstSize = freeCount;
            }
            currentStart = -1;
            freeCount = 0;
        }
        i++;
    }

    if (worstStart != -1)
    {
        for (int j = worstStart; j < worstStart + size; j++)
        {
            memory[j] = adrs;
        }
        return true;
    }

    return false; // Allocation failed
}

void clear(unsigned int adrs)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (memory[i] == adrs)
        {
            memory[i] = 0;
        }
    }
}
void run_allocation_algorithm(const char *algorithm_name)
{
    FILE *file = fopen("queries.txt", "r");
    if (!file)
    {
        perror("Failed to open queries.txt");
        return;
    }

    char query[20];
    unsigned int adrs, size;

    clock_t start_time = clock(); // Start the timer
    int query_count = 0;

    while (fscanf(file, "%s", query) == 1)
    {
        if (strcmp(query, "end") == 0)
        {
            break;
        }
        else if (strcmp(query, "allocate") == 0)
        {
            query_count++; // Increment query count
            fscanf(file, "%s %u %u", query, &adrs, &size);
            if (strcmp(query, algorithm_name) == 0)
            {
                if (strcmp(algorithm_name, "first_fit") == 0)
                {
                    allocate_first_fit(adrs, size);
                }
                else if (strcmp(algorithm_name, "best_fit") == 0)
                {
                    allocate_best_fit(adrs, size);
                }
                else if (strcmp(algorithm_name, "worst_fit") == 0)
                {
                    allocate_worst_fit(adrs, size);
                }
            }
        }
        else if (strcmp(query, "clear") == 0)
        {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        }
    }

    clock_t end_time = clock(); // Stop the timer
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    fclose(file);

    // Write performance results
    FILE *results_file = fopen("allocator_res.txt", "a");
    if (!results_file)
    {
        perror("Failed to open ex1.txt");
        return;
    }
    fprintf(results_file, "Algorithm: %s\n", algorithm_name);
    fprintf(results_file, "Total Allocation Time: %.3f seconds\n", elapsed_time);

    // Calculate and print throughput
    double throughput = query_count / elapsed_time;
    fprintf(results_file, "Throughput: %.2f queries/second\n", throughput);

    fclose(results_file);
}

int main()
{
    // Clear ex1.txt if it exists
    FILE *results_file = fopen("allocator_res.txt", "w");
    fclose(results_file);

    run_allocation_algorithm("first_fit");
    run_allocation_algorithm("best_fit");
    run_allocation_algorithm("worst_fit");

    return 0;
}