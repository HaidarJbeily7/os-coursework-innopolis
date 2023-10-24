#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ctype.h>

int main() {
    const char* input_file = "/dev/random";
    const char* output_file = "text.txt";
    const off_t file_size = 500LL * 1024 * 1024;  // 500 MiB
    const size_t page_size = sysconf(_SC_PAGESIZE);
    const size_t chunk_size = 1024 * page_size;

    int input_fd, output_fd;
    char *file_data;
    size_t capital_count = 0;
    
    // Open the input file
    input_fd = open(input_file, O_RDONLY);
    
    // Create an empty output file
    output_fd = open(output_file, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(output_fd, file_size);

    // Memory map the output file
    file_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, output_fd, 0);

    if (file_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    size_t file_offset = 0;
    char c;
    while (file_offset < file_size) {
        ssize_t bytes_read = read(input_fd, &c, 1);

        if (bytes_read == -1) {
            perror("read");
            break;
        }

        if (isprint(c)) {
            file_data[file_offset] = c;
            if (isupper(c)) {
                capital_count++;
            }
            file_offset++;
        }

        if (file_offset % chunk_size == 0) {
            // Flush changes to disk at 4 MiB intervals
            msync(file_data, chunk_size, MS_SYNC);
        }
    }

    // Print the total number of capital letters
    printf("Total capital letters: %zu\n", capital_count);

    // Unmap the memory
    munmap(file_data, file_size);

    close(input_fd);
    close(output_fd);

    return 0;
}