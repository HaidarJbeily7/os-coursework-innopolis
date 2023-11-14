#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int inotify_fd;
char monitored_dir[PATH_MAX];

void print_directory_stat(const char *dir)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat entry_stat;
    char path[PATH_MAX];

    while ((entry = readdir(d)) != NULL)
    {
        snprintf(path, PATH_MAX, "%s/%s", dir, entry->d_name);
        if (stat(path, &entry_stat) != 0)
            continue;

        printf("Entry: %s - Inode: %ld - Size: %ld bytes\n",
               path, entry_stat.st_ino, entry_stat.st_size);
    }

    closedir(d);
}

void signal_handler(int sig)
{
    printf("\nSIGINT received, printing stat info before exit.\n");
    print_directory_stat(monitored_dir);
    close(inotify_fd);
    exit(EXIT_SUCCESS);
}

void handle_events(int fd, const char *dir)
{
    char buffer[4096]
        __attribute__((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    while (1)
    {
        len = read(fd, buffer, sizeof(buffer));
        if (len == -1 && errno != EAGAIN)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        for (ptr = buffer; ptr < buffer + len;
             ptr += sizeof(struct inotify_event) + event->len)
        {
            event = (const struct inotify_event *)ptr;

            // Process the event
            if (event->len)
            {
                if (event->mask & IN_ACCESS)
                {

                    printf("The file %s was Accessed with WD %d\n", event->name, event->wd);
                }

                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("The directory %s was Created.\n", event->name);
                    }
                    else
                    {
                        printf("The file %s was Created with WD %d\n", event->name, event->wd);
                    }
                }

                if (event->mask & IN_DELETE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("The directory %s was Deleted.\n", event->name);
                    }
                    else
                    {
                        printf("The file %s was Deleted with WD %d\n", event->name, event->wd);
                    }
                }
                if (event->mask & IN_MODIFY)
                {
                    printf("The file %s was modified.\n", event->name);
                }
                if (event->mask & IN_OPEN)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("The directory %s was Opened.\n", event->name);
                    }
                    else
                    {
                        printf("The file %s was Opened.\n", event->name);
                    }
                }
                if (event->mask & IN_ATTRIB)
                {
                    printf("The file %s metadata was Changed.\n", event->name);
                }
            }
        }
    }

    int main(int argc, char **argv)
    {
        if (argc != 2)
        {
            fprintf(stderr, "Usage: %s <path to monitor>\n", argv[0]);
            return EXIT_FAILURE;
        }

        strncpy(monitored_dir, argv[1], PATH_MAX);

        // Setup signal handler
        signal(SIGINT, signal_handler);

        inotify_fd = inotify_init();
        if (inotify_fd < 0)
        {
            perror("inotify_init");
            return EXIT_FAILURE;
        }

        int wd = inotify_add_watch(inotify_fd, argv[1], IN_ALL_EVENTS);
        if (wd == -1)
        {
            perror("inotify_add_watch");
            close(inotify_fd);
            return EXIT_FAILURE;
        }

        // Monitoring loop
        handle_events(inotify_fd, argv[1]);

        close(inotify_fd);
        return EXIT_SUCCESS;
    }
