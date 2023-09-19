#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_STRING_SIZE = 256;

int tribonacci(int n){
    if(n == 2 || n == 1) return 1;
    if(n == 0)return 0;
    int res = 0;
    int bbb = 0;
    int bb = 1;
    int b = 1;
    for(int i = 3 ;i <= n;i++){
        res = bbb + bb + b;
        bbb = bb;
        bb = b;
        b = res;
    }
    return res;
}

int main()
{
    printf("tribonacci(4)= %d\n", tribonacci(4));
    printf("tribonacci(36)= %d\n", tribonacci(36));
    return 0;
}
