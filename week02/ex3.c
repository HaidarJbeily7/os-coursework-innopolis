#include <stdio.h>
#include <stdbool.h> 
#include <string.h>

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}


// Function to check if a number is within a specified range
bool isInRange(int num, int min, int max) {
    return num >= min && num <= max;
}

// Function to convert a number from one base to another
void convert(long long x, int s, int t) {
    // Check if the bases are within the valid range (2 to 10)
    if (!isInRange(s, 2, 10) || !isInRange(t, 2, 10)) {
        printf("Cannot convert! Bases must be between 2 and 10.\n");
        return;
    }

    long long decimal = 0;
    long long multi = 1;
    
    // Convert the number to decimal
    while (x > 0) {
        int digit = x % 10;
        if (digit >= s) {
            printf("Cannot convert! Digit exceeds the source base.\n");
            return;
        }
        decimal += digit * multi;
        multi *= s;
        x /= 10;
    }

    char obtained[100];
    int index = 0;
    
    // Convert the decimal value to the target base
    while (decimal > 0) {
        int digit = decimal % t;
        obtained[index++] = digit + '0';
        decimal /= t;
    }

    if (index == 0) {
        obtained[index++] = '0';
    }

    // Print the result in reverse order
    printf("%s\n", strrev(obtained));
    
}

int main() {
    long long number;
    int source, target;

    printf("Number: ");
    if (scanf("%lld", &number) != 1 || number < 0) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Source: ");
    if (scanf("%d", &source) != 1) {
        printf("Invalid input!\n");
        return 1;
    }

    printf("Target: ");
    if (scanf("%d", &target) != 1) {
        printf("Invalid input!\n");
        return 1;
    }

    convert(number, source, target);

    return 0;
}
