#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

// Define key codes for shortcuts
#define KEY_E 0x12
#define KEY_X 0x2D
#define KEY_P 0x19
#define KEY_C 0x2E
#define KEY_A 0x1E
// Add your custom keys here

// Function to check if a specific key event occurred
int is_key_event(struct input_event *ev, int type, int code)
{
    return ev->type == EV_KEY && ev->code == code && ev->value == type;
}

int main()
{
    int fd, rd;
    struct input_event ev[64];
    int size = sizeof(struct input_event);
    int keys[256] = {0}; // Array to keep track of key states

    // Define your custom shortcut and message here
    char *custom_message = "Your custom message here";
    int custom_keys[] = {/* Your custom key codes here */};
    int custom_key_count = sizeof(custom_keys) / sizeof(custom_keys[0]);

    // Open the keyboard device file
    fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
    if (fd == -1)
    {
        perror("ERROR: Unable to open /dev/input/by-path/platform-i8042-serio-0-event-kbd");
        exit(EXIT_FAILURE);
    }

    printf("Shortcuts:\n");
    printf("P+E → 'I passed the Exam!'\n");
    printf("C+A+P → 'Get some cappuccino!'\n");
    printf("Your custom shortcut → '%s'\n", custom_message);

    // Main loop
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

                // Print event
                if (ev[i].value == 1 || ev[i].value == 2) // PRESSED or REPEATED
                    printf("PRESSED 0x%04x (%d)\n", ev[i].code, ev[i].code);
                else if (ev[i].value == 0) // RELEASED
                    printf("RELEASED 0x%04x (%d)\n", ev[i].code, ev[i].code);

                // Check for shortcut conditions
                if (keys[KEY_P] && keys[KEY_E])
                    printf("I passed the Exam!\n");
                if (keys[KEY_C] && keys[KEY_A] && keys[KEY_P])
                    printf("Get some cappuccino!\n");
                // Add your custom shortcut condition and message here

                // Exit condition
                if (keys[KEY_E] && keys[KEY_X])
                    goto end;
            }
        }
    }

end:
    close(fd);
    return 0;
}
