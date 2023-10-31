#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rusage usage;

    for(int i = 0; i < 10; i++) {
        // Allocate 10 MB of memory
        char *ptr = malloc(10 * 1024 * 1024);
        
        if(!ptr) {
            perror("Memory allocation failed");
            return 1;
        }

        // Fill it with zeros
        memset(ptr, 0, 10 * 1024 * 1024);

        // Get memory usage
        if(getrusage(RUSAGE_SELF, &usage) == 0) {
            printf("Memory usage: %ld kilobytes\n", usage.ru_maxrss);
        } else {
            perror("getrusage failed");
        }

        // Sleep for 1 second
        sleep(1);
    }
    
    return 0;
}