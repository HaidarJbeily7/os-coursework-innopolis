#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 2048
#define MAX_FILENAME_LENGTH 63
#define MAX_FILE_DATA_SIZE 1024

struct Directory;
struct File;

typedef struct Directory Directory;
typedef struct File File;

struct File
{
    unsigned long long int id;
    char name[MAX_FILENAME_LENGTH + 1];
    int size;
    char data[MAX_FILE_DATA_SIZE];
    struct Directory *directory; // The parent directory
};

struct Directory
{
    char name[MAX_FILENAME_LENGTH + 1];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
    struct Directory *directories[8];
    File *files[1024];
};

// Implementation: Operations on files

// Overwrite the File file with the content
void overwrite_to_file(File *file, const char *content)
{
    strcpy(file->data, content);
    file->size = strlen(file->data);
}

// Appends the content to the File file
void append_to_file(File *file, const char *content)
{
    strcat(file->data, content);
    file->size = strlen(file->data);
}

// Prints the path of the File file
void printp_file(File *file)
{

    printf("%s/%s\n", file->directory->path, file->name);
}

// Operations on directories

// Adds the File file to the Directory dir
void add_file(File *file, Directory *dir)
{
    file->directory = dir;
    dir->files[dir->nf] = file;
    dir->nf = dir->nf + 1;
    file->id = dir->nf + strlen(file->name);
    file->size = sizeof(file->data);
}

// Given to you
// Adds the subdirectory dir1 to the directory dir2
void add_dir(Directory *dir1, Directory *dir2)
{
    if (dir1 && dir2)
    {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        if (strcmp(dir2->path, "/"))
        {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
        else
        {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

// Shows details of the File file
void show_file_detailed(File *file)
{
    printf("\nFILE\n");
    printf(" id: %llu\n", file->id);
    printf(" name: %s\n", file->name);
    printf(" size: %d\n", file->size);
    printf(" data:\n");
    printf("    [ %s ]\n", file->data);
}

// Prints the name of the File file
void show_file(File *file)
{
    printf("%s ", file->name);
}

// Displays the content of the Directory dir
void show_dir(Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++)
    {
        show_file(dir->files[i]);
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++)
    {
        show_dir(dir->directories[i]);
    }
    printf("}\n");
}

int main()
{
    Directory home, bin, root;
    home.nf = 0;
    home.nd = 0;

    bin.nf = 0;
    bin.nd = 0;

    root.nf = 0;
    root.nd = 0;

    strcpy(home.name, "home");
    strcpy(bin.name, "bin");
    strcpy(root.name, "root");
    strcpy(root.path, "/");

    // Add subdirectories
    add_dir(&home, &root);
    add_dir(&bin, &root);

    File bash, ex31, ex32;

    strcpy(bash.name, "bash");
    strcpy(ex31.name, "ex3_1.c");
    strcpy(ex32.name, "ex3_2.c");

    add_file(&bash, &bin);
    add_file(&ex31, &home);
    add_file(&ex32, &home);

    overwrite_to_file(&ex31, "int printf(const char * format, ...);");
    overwrite_to_file(&ex32, "//This is a comment in C language");
    overwrite_to_file(&bash, "Bourne Again Shell!");

    append_to_file(&ex31, "int main(){printf('Hello World');}");

    show_dir(&root);
    show_file_detailed(&bash);
    show_file_detailed(&ex31);
    show_file_detailed(&ex32);

    printf("\n\nFile Paths:\n\n");

    printp_file(&bash);
    printp_file(&ex31);
    printp_file(&ex32);

    return EXIT_SUCCESS;
}
