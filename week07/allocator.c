#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MEMORY_SIZE 10000000

unsigned int memory[MEMORY_SIZE];

// Initialize memory cells to zeros
void initialize_memory() {
    for(int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
}

bool allocate_first_fit(unsigned int adrs, unsigned int size) {
    for(int i = 0; i <= MEMORY_SIZE - size; i++) {
        bool isAvailable = true;
        for(int j = 0; j < size; j++) {
            if(memory[i + j] != 0) {
                isAvailable = false;
                break;
            }
        }
        if(isAvailable) {
            for(int j = 0; j < size; j++) {
                memory[i + j] = adrs;
            }
            return true;
        }
    }
    return false;  // Allocation failed
}

bool allocate_best_fit(unsigned int adrs, unsigned int size) {
    int bestStart = -1;
    int bestSize = INT_MAX;
    int currentStart = -1;
    int freeCount = 0;

    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(memory[i] == 0) {
            if(currentStart == -1) currentStart = i;
            freeCount++;
        } else {
            if(freeCount >= size && freeCount < bestSize) {
                bestStart = currentStart;
                bestSize = freeCount;
            }
            freeCount = 0;
            currentStart = -1;
        }
    }

    // Check if we found a slot
    if(bestStart != -1) {
        for(int j = 0; j < size; j++) {
            memory[bestStart + j] = adrs;
        }
        return true;
    }
    return false;  // Allocation failed
}

bool allocate_worst_fit(unsigned int adrs, unsigned int size) {
    int worstStart = -1;
    int worstSize = -1;
    int currentStart = -1;
    int freeCount = 0;

    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(memory[i] == 0) {
            if(currentStart == -1) currentStart = i;
            freeCount++;
        } else {
            if(freeCount >= size && freeCount > worstSize) {
                worstStart = currentStart;
                worstSize = freeCount;
            }
            freeCount = 0;
            currentStart = -1;
        }
    }

    // Check if we found a slot
    if(worstStart != -1) {
        for(int j = 0; j < size; j++) {
            memory[worstStart + j] = adrs;
        }
        return true;
    }
    return false;  // Allocation failed
}

void clear(unsigned int adrs) {
    for(int i = 0; i < MEMORY_SIZE; i++) {
        if(memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

int main() {
    initialize_memory();

    FILE *file = fopen("queries.txt", "r");
    if(!file) {
        perror("Failed to open queries.txt");
        return 1;
    }

    char query[20];
    unsigned int adrs, size;
    while(fscanf(file, "%s", query) == 1) {
        if(strcmp(query, "end") == 0) {
            break;
        } else if(strcmp(query, "allocate") == 0) {
            fscanf(file, "%s %u %u", query, &adrs, &size);
            if(strcmp(query, "first_fit") == 0) {
                allocate_first_fit(adrs, size);
            } else if(strcmp(query, "best_fit") == 0) {
                allocate_best_fit(adrs, size);
            } else if(strcmp(query, "worst_fit") == 0) {
                allocate_worst_fit(adrs, size);
            }
        } else if(strcmp(query, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        }
    }
    fclose(file);

    return 0;
}