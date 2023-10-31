#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

    FILE *f = fopen("/tmp/ex1.pid", "w");
    if(!f) {
        perror("Error opening /tmp/ex1.pid");
        exit(1);
    }

    fprintf(f, "%d", getpid());
    fclose(f);

    char *mmap_ptr;
    char password[14];

    mmap_ptr = mmap(NULL, 14, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap_ptr == MAP_FAILED) {
        perror("Error with mmap");
        exit(1);
    }

    strncpy(mmap_ptr, "pass:", 5);

    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData == -1) {
        perror("Error opening /dev/urandom");
        exit(1);
    }

    for(int i=5; i<13; i++) { // Adjusted loop for a 7-character password
        char ch;
        while(1) {
            if(read(randomData, &ch, 1) != 1) {
                perror("Error reading /dev/urandom");
                exit(1);
            }
            if(ch >= (int)('A') && ch <= (int)('z')) {
                password[i] = ch;
                mmap_ptr[i] = ch;
                break;
            }
        }
    }
    password[13] = '\0';
    mmap_ptr[13] = '\0'; // Terminate the mmap_ptr as well

    close(randomData);


    printf("%s\n", mmap_ptr);
    while(1) {
        sleep(1);
    }
    munmap(mmap_ptr, 14);

    return 0;
}