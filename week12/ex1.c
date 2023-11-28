#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

int main()
{
    int fd, rd;
    struct input_event ev[64];
    int size = sizeof(struct input_event);
    
    int keys[256] = {0}; 
    char *custom_message = "It's 11, basic addition!";
    int custom_keys[] = {KEY_9, KEY_2};
    int custom_key_count = sizeof(custom_keys) / sizeof(custom_keys[0]);

    fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
    if (fd == -1)
    {
        perror("ERROR: Unable to open /dev/input/by-path/platform-i8042-serio-0-event-kbd");
        exit(EXIT_FAILURE);
    }

    printf("Shortcuts:\n");
    printf("P+E → 'I passed the Exam!'\n");
    printf("C+A+P → 'Get some cappuccino!'\n");
    printf("E+X → Exit the program\n");
    printf("2+9 → '%s'\n", custom_message);

    while (1)
    {
        rd = read(fd, ev, size * 64);
        if (rd < (int)size)
        {
            perror("ERROR: Read");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rd / size; i++)
        {
            if (ev[i].type == EV_KEY)
            {
                keys[ev[i].code] = ev[i].value;

                if(ev[i].value == 2)
                    printf("\nREPEATED 0x%04x (%d)\n", ev[i].code, ev[i].code);
                if (ev[i].value ==  1)
                    printf("\nPRESSED 0x%04x (%d)\n", ev[i].code, ev[i].code);
                else if (ev[i].value == 0) 
                    printf("\nRELEASED 0x%04x (%d)\n", ev[i].code, ev[i].code);

                
                if (keys[KEY_P] && keys[KEY_E])
                    printf("I passed the Exam!\n");
                if (keys[KEY_C] && keys[KEY_A] && keys[KEY_P])
                    printf("Get some cappuccino!\n");
                if(keys[KEY_9] && keys[KEY_2])
                    printf("%s\n", custom_message);
                if (keys[KEY_E] && keys[KEY_X])
                    goto end;
            }
        }
    }

end:
    close(fd);
    return 0;
}
