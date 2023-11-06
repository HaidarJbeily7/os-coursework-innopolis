#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define PAGETABLE "/tmp/ex2/pagetable"
#define PAGE_SIZE 8

int waiter = 1;

struct PTE
{
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

typedef enum
{
    RANDOM, // Enum value for random page replacement
    NFU,    // Enum value for Not Frequently Used page replacement
    AGING   // Enum value for Aging page replacement
} ReplacementAlgorithm;

ReplacementAlgorithm replacement_algorithm;
char **RAM, **disk;
int *free_frames, num_frames, num_pages;
struct PTE *page_table;
int disk_accesses = 0;

void initialize_disk(int num_pages)
{
    srand(time(NULL));

    for (int i = 0; i < num_pages; i++)
    {
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            disk[i][j] = (char)(' ' + rand() % 95);
        }
    }
}

unsigned char *age_counters; // For aging algorithm
int *reference_counters;     // For NFU algorithm

// Random page replacement
int random_page(struct PTE *page_table, int num_pages)
{
    int victim;
    do
    {
        victim = rand() % num_pages;
    } while (!page_table[victim].valid);

    return victim;
}

// NFU page replacement
int nfu(struct PTE *page_table, int num_pages)
{
    int victim = 0;
    for (int i = 1; i < num_pages; i++)
    {
        if (page_table[i].valid && (reference_counters[i] < reference_counters[victim]))
        {
            victim = i;
        }
    }
    return victim;
}

// Aging page replacement
int aging(struct PTE *page_table, int num_pages)
{
    int victim = 0;
    for (int i = 0; i < num_pages; i++)
    {

        age_counters[i] = age_counters[i] >> 1;

        if (page_table[i].referenced)
        {
            age_counters[i] = age_counters[i] | (1 << 7);
        }

        if (page_table[i].valid && (age_counters[i] < age_counters[victim]))
        {
            victim = i;
        }

        page_table[i].referenced = 0;
    }
    return victim;
}

void sigusr1_handler(int signum)
{

    printf("\n\n####### Pager #######\n\n");
    // Find the first invalid entry in the page table
    int invalid_page = -1;
    int pid = 0;
    for (int i = 0; i < num_pages; i++)
    {
        if (page_table[i].referenced)
        {
            invalid_page = i;
            pid = page_table[i].referenced;
            break;
        }
    }

    if (invalid_page == -1)
    {
        printf("%d disk accesses in total\n", disk_accesses);
        printf("Pager is terminated");
        waiter = 0;
        return;
    }

    printf("A disk access request from MMU Process (pid=%d)\n", pid);
    printf("Page %d is referenced\n", invalid_page);

    int frame_to_use = -1;
    for (int i = 0; i < num_frames; i++)
    {
        if (free_frames[i] == 1)
        {
            frame_to_use = i;
            break;
        }
    }

    if (frame_to_use == -1)
    {
        printf("We do not have free frames in RAM\n");
        int victim;
        for (int i = 0; i < num_pages; i++)
        {
            if (page_table[i].valid)
            {
                if (page_table[i].dirty)
                    disk_accesses++;
                frame_to_use = page_table[i].frame;
                victim = i;
                page_table[i].valid = false;
                page_table[i].referenced = 0;
                page_table[i].frame = -1;
                page_table[i].dirty = false;
                break;
            }
        }
        // Example usage in your replacement logic (pseudo-code)
        int victim_page = -1;
        switch (replacement_algorithm)
        {
        case RANDOM:
            victim_page = random_page(page_table, num_pages);
            break;
        case NFU:
            victim_page = nfu(page_table, num_pages);
            break;
        case AGING:
            victim_page = aging(page_table, num_pages);
            break;
        }

        printf("Chose a random victim page %d\n", victim);
        printf("Replace/Evict it with page %d to be allocated to frame %d\n", invalid_page, frame_to_use);
    }
    else
    {
        printf("We can allocate it to free frame %d\n", frame_to_use);
    }

    printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", invalid_page, frame_to_use);
    // Copy the data from disk to RAM
    memcpy(RAM[frame_to_use], disk[invalid_page], PAGE_SIZE);

    // Update the page table entry for the invalid_page
    page_table[invalid_page].valid = true;
    page_table[invalid_page].frame = frame_to_use;
    page_table[invalid_page].referenced = 0; // Reset the referenced PID

    // Mark the frame as used
    free_frames[frame_to_use] = 0;

    // Increase disk access count
    disk_accesses++;

    printf("RAM array\n");
    for (int i = 0; i < num_frames; i++)
    {
        printf("Frame %d --> %s\n", i, RAM[i]);
    }
    printf("disck access is %d so far\n", disk_accesses);
    printf("Resume MMU process\n");
    printf("------------------------------\n\n\n");
    kill(pid, SIGCONT);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <num_pages> <num_frames>\n", argv[0]);
        exit(1);
    }

    num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);

    const char *algorithm_str = argv[3];

    if (strcmp(algorithm_str, "random") == 0)
    {
        replacement_algorithm = RANDOM;
    }
    else if (strcmp(algorithm_str, "nfu") == 0)
    {
        replacement_algorithm = NFU;
    }
    else if (strcmp(algorithm_str, "aging") == 0)
    {
        replacement_algorithm = AGING;
    }
    else
    {
        fprintf(stderr, "Invalid page replacement algorithm specified\n");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGUSR1 handler
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    age_counters = calloc(num_pages, sizeof(unsigned char));
    reference_counters = calloc(num_pages, sizeof(int));

    printf("\n\n####### Pager #######\n\n");

    // Initialize RAM and disk
    RAM = (char **)malloc(num_frames * sizeof(char *));
    for (int i = 0; i < num_frames; i++)
    {
        RAM[i] = (char *)malloc(PAGE_SIZE * sizeof(char));
    }
    disk = (char **)malloc(num_pages * sizeof(char *));
    for (int i = 0; i < num_pages; i++)
    {
        disk[i] = (char *)malloc(PAGE_SIZE * sizeof(char));
    }
    initialize_disk(num_pages);

    // Initialize free frames list
    free_frames = (int *)malloc(num_frames * sizeof(int));
    for (int i = 0; i < num_frames; i++)
    {
        free_frames[i] = 1; // 1 means the frame is free
    }

    // Open and map the page table
    int fd = open(PAGETABLE, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("Error opening page table file");
        exit(1);
    }
    ftruncate(fd, num_pages * sizeof(struct PTE));
    page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED)
    {
        perror("Error mapping page table");
        close(fd);
        exit(1);
    }

    // Initially, all pages on disk
    for (int i = 0; i < num_pages; i++)
    {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++)
    {
        printf("Page %d ---> valid:%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("------------------------------\n");

    printf("Initialized RAM\nRAM array\n");
    for (int i = 0; i < num_frames; i++)
    {
        printf("Frame %d --> %s\n", i, RAM[i]);
    }
    printf("------------------------------\n");

    printf("Initialized disk\nDisk array\n");
    for (int i = 0; i < num_pages; i++)
    {
        printf("Page %d --> %s\n", i, disk[i]);
    }
    printf("------------------------------\n\n\n");

    while (waiter)
    {
    }

    free(age_counters);
    free(reference_counters);

    return 0;
}