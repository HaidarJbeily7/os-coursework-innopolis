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

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

char **RAM, **disk;
int *free_frames, num_frames;
struct PTE *page_table;
int disk_accesses = 0;


void initialize_disk(int num_pages) {
    srand(time(NULL));

    for (int i = 0; i < num_pages; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            disk[i][j] = (char) (' ' + rand() % 95);
        }
    }
}
void sigusr1_handler(int signum) {
    (void) signum; // To avoid unused parameter warning

    // Find the first invalid entry in the page table
    int invalid_page = -1;
    for (int i = 0; i < num_frames; i++) {
        if (!page_table[i].valid) {
            invalid_page = i;
            break;
        }
    }

    if (invalid_page == -1) {
        fprintf(stderr, "Error: Could not find an invalid page!\n");
        return;
    }

    // Swap the page from the disk to RAM (a basic swap-in without considering swap-out)
    // Let's assume the first frame in free_frames will be used
    int frame_to_use = -1;
    for (int i = 0; i < num_frames; i++) {
        if (free_frames[i] == 1) {
            frame_to_use = i;
            break;
        }
    }

    if (frame_to_use == -1) {
        fprintf(stderr, "Error: No free frame to use!\n");
        return;
    }

    // Copy the data from disk to RAM
    memcpy(RAM[frame_to_use], disk[invalid_page], PAGE_SIZE);

    // Update the page table entry for the invalid_page
    page_table[invalid_page].valid = true;
    page_table[invalid_page].frame = frame_to_use;
    page_table[invalid_page].referenced = -1; // Reset the referenced PID

    // Mark the frame as used
    free_frames[frame_to_use] = 0;

    // Increase disk access count
    disk_accesses++;

    printf("Handled page fault for page %d. Loaded into frame %d.\n", invalid_page, frame_to_use);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_pages> <num_frames>\n", argv[0]);
        exit(1);
    }

    int num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);

    // Setting up SIGUSR1 handler
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    // Initialize RAM and disk
    RAM = (char **) malloc(num_frames * sizeof(char *));
    for (int i = 0; i < num_frames; i++) {
        RAM[i] = (char *) malloc(PAGE_SIZE * sizeof(char));
    }
    disk = (char **) malloc(num_pages * sizeof(char *));
    for (int i = 0; i < num_pages; i++) {
        disk[i] = (char *) malloc(PAGE_SIZE * sizeof(char));
    }
    initialize_disk(num_pages);
    
    // Initialize free frames list
    free_frames = (int *) malloc(num_frames * sizeof(int));
    for (int i = 0; i < num_frames; i++) {
        free_frames[i] = 1; // 1 means the frame is free
    }

    // Open and map the page table
    int fd = open(PAGETABLE, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening page table file");
        exit(1);
    }
    ftruncate(fd, num_pages * sizeof(struct PTE));
    page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping page table");
        close(fd);
        exit(1);
    }

    // Initially, all pages on disk
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = -1;
    }

    while (1) {
        pause(); // Wait for signals
    }
    return 0;
}