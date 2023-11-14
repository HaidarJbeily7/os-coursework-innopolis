#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

void find_all_hlinks(const char *dir, const char *source)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat source_stat, entry_stat;
    char path[PATH_MAX];

    if (stat(source, &source_stat) != 0)
    {
        perror("stat");
        return;
    }

    while ((entry = readdir(d)) != NULL)
    {
        snprintf(path, PATH_MAX, "%s/%s", dir, entry->d_name);
        if (stat(path, &entry_stat) != 0)
            continue;

        if (S_ISREG(entry_stat.st_mode) && entry_stat.st_ino == source_stat.st_ino)
        {
            printf("Hard link found: %s (Inode: %ld)\n", path, entry_stat.st_ino);
        }
    }

    closedir(d);
}

void unlink_all(const char *dir, const char *source)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat source_stat, entry_stat;
    char path[PATH_MAX];
    ino_t source_ino;

    if (stat(source, &source_stat) != 0)
    {
        perror("stat");
        return;
    }

    source_ino = source_stat.st_ino;

    while ((entry = readdir(d)) != NULL)
    {
        snprintf(path, PATH_MAX, "%s/%s", dir, entry->d_name);
        if (stat(path, &entry_stat) != 0)
            continue;

        if (S_ISREG(entry_stat.st_mode) && entry_stat.st_ino == source_ino)
        {
            if (strcmp(path, source) != 0)
            { // Only unlink if not the source file
                unlink(path);
                printf("Removed hard link: %s\n", path);
            }
        }
    }

    closedir(d);
}

void create_sym_link(const char *source, const char *link)
{
    if (symlink(source, link) != 0)
    {
        perror("symlink");
    }
    else
    {
        printf("Symbolic link created: %s -> %s\n", link, source);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *dir = argv[1];
    char file_path[PATH_MAX];
    char link_path[PATH_MAX];
    FILE *file;

    snprintf(file_path, PATH_MAX, "%s/myfile1.txt", dir);
    file = fopen(file_path, "w");
    if (!file)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(file, "Hello world.\n");
    fclose(file);

    // Create hard links myfile11.txt and myfile12.txt
    snprintf(link_path, PATH_MAX, "%s/myfile11.txt", dir);
    if (link(file_path, link_path) != 0)
    {
        perror("link");
        return EXIT_FAILURE;
    }

    snprintf(link_path, PATH_MAX, "%s/myfile12.txt", dir);
    if (link(file_path, link_path) != 0)
    {
        perror("link");
        return EXIT_FAILURE;
    }

    // Find all hard links to myfile1.txt
    find_all_hlinks(dir, file_path);

    // Move myfile1.txt to /tmp/myfile1.txt
    char tmp_path[PATH_MAX] = "/tmp/myfile1.txt";
    if (rename(file_path, tmp_path) != 0)
    {
        perror("rename");
        return EXIT_FAILURE;
    }

    snprintf(link_path, PATH_MAX, "%s/myfile11.txt", dir);
    file = fopen(link_path, "a");
    if (!file)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(file, "Additional content.\n");
    fclose(file);

    // Create a symbolic link myfile13.txt
    snprintf(link_path, PATH_MAX, "%s/myfile13.txt", dir);
    create_sym_link(tmp_path, link_path);
    file = fopen(link_path, "a");
    if (!file)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(file, "Additional content.\n");
    fclose(file);

    snprintf(link_path, PATH_MAX, "/tmp/myfile1.txt");
    file = fopen(link_path, "a");
    if (!file)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fprintf(file, "Additional content.\n");
    fclose(file);

    // Remove all duplicates of hard links to myfile11.txt only in the watched directory
    unlink_all(dir, file_path);

    // Print the stat info of the kept hard link and check the number of hard links
    struct stat statbuf;
    if (stat(link_path, &statbuf) != 0)
    {
        perror("stat");
        return EXIT_FAILURE;
    }
    printf("Remaining hard link: %s\nNumber of Links: %lu\n", link_path, statbuf.st_nlink);

    return EXIT_SUCCESS;
}
