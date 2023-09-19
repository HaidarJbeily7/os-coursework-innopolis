#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_STRING_SIZE = 256;

int count(char string[], char k){
    int res = 0 ;
    int st_size = strlen(string);
    for(int i=0; i < st_size; i++){
        if(tolower(string[i]) == k)res++;
    }
    return res;
}

void countAll(char string[]){
    int st_size = strlen(string);
    for(int i=0; i < st_size; i++){
        printf("%c:%d", tolower(string[i]), count(string, tolower(string[i])));
        if(i+1 != st_size)printf(", ");
    }
    printf("\n");
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Please input string from the command line! \n");
        return 1;
    }

    countAll(argv[1]);

    return 0;
}
