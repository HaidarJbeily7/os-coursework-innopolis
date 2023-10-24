#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_SIZE (500 * 1024 * 1024) // 500 MiB
#define PAGE_SIZE sysconf(_SC_PAGESIZE)
#define CHUNK_SIZE (1024 * PAGE_SIZE)

int main()
{
    // Step 1: Create and open the text.txt file
    int text_fd = open("text.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (text_fd == -1)
    {
        perror("Failed to open text.txt");
        return 1;
    }

    // Truncate the file to the desired size
    if (ftruncate(text_fd, FILE_SIZE) == -1)
    {
        perror("Failed to truncate the file");
        return 1;
    }

    // Step 2: Open /dev/random
    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1)
    {
        perror("Failed to open /dev/random");
        return 1;
    }

    // Memory map the text.txt file
    char *file_memory = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, 0);
    if (file_memory == MAP_FAILED)
    {
        perror("Failed to memory map the file");
        return 1;
    }

    // Step 3: Read and process data from /dev/random
    char *current_position = file_memory;
    int capital_count = 0;

    for (off_t i = 0; i < FILE_SIZE; i++)
    {
        char c;
        if (read(random_fd, &c, 1) == -1)
        {
            perror("Failed to read from /dev/random");
            return 1;
        }

        if (isprint(c))
        {
            *current_position = c;
            if (isupper(c))
            {
                capital_count++;
            }
            current_position++;
        }

        if ((i + 1) % CHUNK_SIZE == 0)
        {
            // Reached a chunk boundary, unmap and remap
            munmap(file_memory, FILE_SIZE);
            file_memory = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, 0);
            if (file_memory == MAP_FAILED)
            {
                perror("Failed to remap the file");
                return 1;
            }
            current_position = file_memory + i + 1;
        }
    }

    printf("Total capital letters: %d\n", capital_count);

    // Step 4: Replace capital letters with lowercase
    for (off_t i = 0; i < FILE_SIZE; i++)
    {
        if (isupper(file_memory[i]))
        {
            file_memory[i] = tolower(file_memory[i]);
        }
    }

    // Unmap the memory
    if (munmap(file_memory, FILE_SIZE) == -1)
    {
        perror("Failed to unmap the file");
        return 1;
    }

    // Close files
    close(text_fd);
    close(random_fd);

    return 0;
}
