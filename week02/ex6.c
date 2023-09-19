#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    
    for(int i=1; i <= 7; i++){
        for(int j = 1; j <= i; j++){
            putchar('*');
        }
        putchar('\n');
    }

    putchar('\n');

    for(int i=1; i <= 4; i++){
        for(int j = 1; j <= i; j++){
            putchar('*');
        }
        putchar('\n');
    }
    for(int i=3; i >=1 ; i--){
        for(int j = 1; j <= i; j++){
            putchar('*');
        }
        putchar('\n');
    }





    putchar('\n');
    for(int i=1; i <= 7; i++){
        for(int j = 1; j <= 7; j++){
            putchar('*');
        }
        putchar('\n');
    }



    return 0;
}
