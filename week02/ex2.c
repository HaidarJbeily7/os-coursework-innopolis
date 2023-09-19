#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_STRING_SIZE = 256;

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

int main()
{
    char string[MAX_STRING_SIZE];
    
    fgets(string, MAX_STRING_SIZE, stdin);
    
    int string_length = strlen(string);

    short is_dot_included = 0;
    int end_index = 0;
    while(end_index < string_length){
        if(string[end_index] == '.'){
            is_dot_included = 1;
            break;
        }
        end_index++;
    }
    
    if(is_dot_included == 0){
        end_index--;
    }

    char substring[MAX_STRING_SIZE];
    strncpy(substring, string, end_index);

    char reversed_string[MAX_STRING_SIZE];
    strcpy(reversed_string, strrev(substring));

    
    printf("\"%s\"\n", reversed_string);

    return 0;
}
