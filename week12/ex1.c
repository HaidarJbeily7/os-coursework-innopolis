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
    
    int status_keys[256] = {0}; // array to follow the status of the keys 
    char *custom_message = "It's 11, basic addition!"; // message of my custom shortcut

    // openning the file to listen to keyboard events
    fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);

    // in case openning failed
    if (fd == -1)
    {
        perror("ERROR: Unable to open /dev/input/by-path/platform-i8042-serio-0-event-kbd");
        exit(EXIT_FAILURE);
    }


    // Printing the list of shortcuts
    printf("Shortcuts:\n");
    printf("P+E → 'I passed the Exam!'\n");
    printf("C+A+P → 'Get some cappuccino!'\n");
    printf("E+X → Exit the program\n");
    printf("2+9 → '%s'\n", custom_message);


    // infinite loop to keep listening to input updates
    while (1)
    {
        rd = read(fd, ev, size * 64); // catching the latest events
        if (rd < (int)size)
        {
            perror("ERROR: Read");
            exit(EXIT_FAILURE);
        }

        // loop through the events
        for (int i = 0; i < rd / size; i++)
        {
            // catch up the key event
            if (ev[i].type == EV_KEY)
            {
                // updating status key array
                status_keys[ev[i].code] = ev[i].value;


                // printing the event 
                if(ev[i].value == 2)
                    printf("\nREPEATED 0x%04x (%d)\n", ev[i].code, ev[i].code);
                if (ev[i].value ==  1)
                    printf("\nPRESSED 0x%04x (%d)\n", ev[i].code, ev[i].code);
                else if (ev[i].value == 0) 
                    printf("\nRELEASED 0x%04x (%d)\n", ev[i].code, ev[i].code);

                // check if one shortcut has clicked and if so, execute
                if (status_keys[KEY_P] && status_keys[KEY_E])
                    printf("I passed the Exam!\n");
                if (status_keys[KEY_C] && status_keys[KEY_A] && status_keys[KEY_P])
                    printf("Get some cappuccino!\n");
                if(status_keys[KEY_9] && status_keys[KEY_2])
                    printf("%s\n", custom_message);
                if (status_keys[KEY_E] && status_keys[KEY_X])
                    goto end; // if E+X, exit the program
            }
        }
    }

end:
    close(fd);
    return EXIT_SUCCESS;
}
