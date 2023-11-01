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


int waiter = 1;
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

void sigcont_handler(int signum) {
    (void) signum; // To avoid unused parameter warning
    waiter = 0;
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
    int fd = open(PAGETABLE, O_RDWR, 0666);
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
    printf("\n\n####### MMU #######\n\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++)
    {
        printf("Page %d ---> valid:%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }

    printf("------------------------------\n");


    for (int i = 2; i < argc - 1; i++) {
        printf("\n\n####### MMU #######\n\n");
        printf("------------------------------\n");
        char mode = argv[i][0];
        char num = argv[i][1];
        int page = (int)(num - '0');
        if (mode == 'W')
        {
            printf("Write Request for page %d\n", page);
        }
        else
        {
            printf("Read Request for page %d\n", page);
        }

        if (page_table[page].valid) {
            printf("It is a valid page\n");
            if (mode == 'W')
            {
                page_table[page].dirty = true;
                printf("It is a write request then set the dirty field\n");
            }
            
        } else {
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page].referenced = getpid();
            kill(pager_pid, SIGUSR1);
            while (/* condition */ waiter)
            {
                /* code */
            }
            waiter = 1;
            printf("MMU resumed by SIGCONT signal from pager\n");
        }


        printf("Page table\n");
        for (int i = 0; i < num_pages; i++)
        {
            printf("Page %d ---> valid:%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }

        printf("------------------------------\n\n");
    }
    kill(pager_pid, SIGUSR1);
    munmap(page_table, num_pages * sizeof(struct PTE));
    close(fd);
    printf("\nDone all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");
    return 0;
}