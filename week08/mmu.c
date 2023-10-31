#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define PAGETABLE "/tmp/ex2/pagetable"
#define PAGE_SIZE 8

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

void sigcont_handler(int signum) {
    (void) signum; // To avoid unused parameter warning
    printf("Received SIGCONT. Page fault handled.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <num_pages> <reference_string> <pager_pid>\n", argv[0]);
        exit(1);
    }
    
    int num_pages = atoi(argv[1]);
    pid_t pager_pid = atoi(argv[argc - 1]);
    
    struct sigaction sa;
    sa.sa_handler = sigcont_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCONT, &sa, NULL);

    // Open and map the page table
    int fd = open(PAGETABLE, O_RDWR);
    if (fd == -1) {
        perror("Error opening page table file");
        exit(1);
    }
    struct PTE *page_table = mmap(NULL, num_pages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Error mapping page table");
        close(fd);
        exit(1);
    }

    for (int i = 2; i < argc - 1; i++) {
        char mode = argv[i][0];
        int page = atoi(argv[i] + 1);

        if (page_table[page].valid) {
            if (mode == 'W') {
                page_table[page].dirty = true;
            }
            // Print the updated page table
        } else {
            // Page fault
            page_table[page].referenced = getpid(); // Setting the referenced field to MMU PID
            kill(pager_pid, SIGUSR1);
            pause(); 
        }
    }
    kill(pager_pid, SIGUSR1);
    munmap(page_table, num_pages * sizeof(struct PTE));
    close(fd);
    return 0;
}