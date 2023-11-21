#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_FILES 16
#define BLOCK_SIZE 1024
#define MAX_FILE_SIZE 8
#define MAX_FILENAME 16
#define INODE_SIZE 56
#define FREE_BLOCK_LIST_SIZE 128
#define SUPERBLOCK_SIZE BLOCK_SIZE

struct inode
{
    char name[MAX_FILENAME];
    int size;
    int blockPointers[MAX_FILE_SIZE];
    int used;
};

int disk_fd;

// Helper function to read and write inodes
struct inode read_inode(int index);
void write_inode(int index, struct inode node);

int create(char name[16], int size)
{
    // Assuming global file descriptor disk_fd is already opened

    // Step 1: Check for free space
    char freeBlockList[FREE_BLOCK_LIST_SIZE];
    lseek(disk_fd, 0, SEEK_SET); // Go to start of the disk
    read(disk_fd, freeBlockList, FREE_BLOCK_LIST_SIZE);

    // ... Code to check for sufficient free blocks ...

    // Step 2: Find a free inode
    struct inode node;
    int inode_index = -1;
    for (int i = 0; i < MAX_FILES; i++)
    {
        node = read_inode(i);
        if (node.used == 0)
        {
            inode_index = i;
            break;
        }
    }
    if (inode_index == -1)
    {
        printf("No free inode available.\n");
        return -1;
    }

    // Initialize inode
    strncpy(node.name, name, MAX_FILENAME);
    node.size = size;
    node.used = 1;

    // Step 3: Allocate data blocks
    for (int i = 0, alloc = 0; i < FREE_BLOCK_LIST_SIZE && alloc < size; i++)
    {
        if (freeBlockList[i] == 0)
        {
            node.blockPointers[alloc++] = i;
            freeBlockList[i] = 1; // Mark block as used
        }
    }

    // Step 4: Write inode and free block list
    lseek(disk_fd, 0, SEEK_SET); // Go to start of the disk
    write(disk_fd, freeBlockList, FREE_BLOCK_LIST_SIZE);
    write_inode(inode_index, node);

    return 0;
}

int delete(char name[16])
{
    char freeBlockList[FREE_BLOCK_LIST_SIZE];
    lseek(disk_fd, 0, SEEK_SET);
    read(disk_fd, freeBlockList, FREE_BLOCK_LIST_SIZE);

    struct inode node;
    for (int i = 0; i < MAX_FILES; i++)
    {
        node = read_inode(i);
        if (node.used == 1 && strcmp(node.name, name) == 0)
        {
            for (int j = 0; j < node.size; j++)
            {
                freeBlockList[node.blockPointers[j]] = 0; // Free the blocks
            }
            node.used = 0; // Mark inode as free

            // Write the free block list and inode back to disk
            lseek(disk_fd, 0, SEEK_SET);
            write(disk_fd, freeBlockList, FREE_BLOCK_LIST_SIZE);
            write_inode(i, node);

            return 0;
        }
    }

    printf("File not found.\n");
    return -1;
}

int ls(void)
{
    struct inode node;
    for (int i = 0; i < MAX_FILES; i++)
    {
        node = read_inode(i);
        if (node.used == 1)
        {
            printf("File: %s, Size: %d blocks\n", node.name, node.size);
        }
    }
    return 0;
}

int read(char name[16], int blockNum, char buf[1024])
{
    struct inode node;
    for (int i = 0; i < MAX_FILES; i++)
    {
        node = read_inode(i);
        if (node.used == 1 && strcmp(node.name, name) == 0)
        {
            if (blockNum >= node.size)
            {
                printf("Block number out of range.\n");
                return -1;
            }
            int blockAddr = node.blockPointers[blockNum];
            lseek(disk_fd, blockAddr * BLOCK_SIZE, SEEK_SET);
            read(disk_fd, buf, BLOCK_SIZE);
            return 0;
        }
    }

    printf("File not found.\n");
    return -1;
}

int write(char name[16], int blockNum, char buf[1024])
{
    struct inode node;
    for (int i = 0; i < MAX_FILES; i++)
    {
        node = read_inode(i);
        if (node.used == 1 && strcmp(node.name, name) == 0)
        {
            if (blockNum >= node.size)
            {
                printf("Block number out of range.\n");
                return -1;
            }
            int blockAddr = node.blockPointers[blockNum];
            lseek(disk_fd, blockAddr * BLOCK_SIZE, SEEK_SET);
            write(disk_fd, buf, BLOCK_SIZE);
            return 0;
        }
    }

    printf("File not found.\n");
    return -1;
}

int main(int argc, char *argv[])
{
    char diskName[32];
    char command;
    char fileName[16];
    int size, blockNum;
    char buffer[1024];

    // Read the disk name from input
    scanf("%s", diskName);

    // Open the disk file
    disk_fd = open(diskName, O_RDWR);
    if (disk_fd < 0)
    {
        perror("Error opening disk file");
        return -1;
    }

    while (scanf(" %c", &command) != EOF)
    {
        switch (command)
        {
        case 'C':
            scanf("%s %d", fileName, &size);
            create(fileName, size);
            break;
        case 'D':
            scanf("%s", fileName);
            delete (fileName);
            break;
        case 'L':
            ls();
            break;
        case 'R':
            scanf("%s %d", fileName, &blockNum);
            read(fileName, blockNum, buffer);
            break;
        case 'W':
            scanf("%s %d", fileName, &blockNum);
            memset(buffer, 0, 1024);
            write(fileName, blockNum, buffer);
            break;
        default:
            printf("Unknown command: %c\n", command);
            break;
        }
    }

    // Close the disk file
    close(disk_fd);

    return 0;
}

struct inode read_inode(int index)
{
    struct inode node;
    lseek(disk_fd, SUPERBLOCK_SIZE + index * sizeof(struct inode), SEEK_SET);
    read(disk_fd, &node, sizeof(struct inode));
    return node;
}

void write_inode(int index, struct inode node)
{
    lseek(disk_fd, SUPERBLOCK_SIZE + index * sizeof(struct inode), SEEK_SET);
    write(disk_fd, &node, sizeof(struct inode));
}
