#include <stdio.h>
#include <stdlib.h>


void const_tri(int * const p, int n){
    if(n < 0){
        printf("No Solution for const_tri(, %d)!", n);
        return;
    }
    if(n<3){
        printf("const_tri(, %d) = %d\n",n , *(p+n));
        return;
    }
    int res = 0;
    for(int i = 3 ;i <= n;i++){
        res = *(p) + *(p+1) + *(p+2);
        *(p) = *(p+1);
        *(p+1) = *(p+2);
        *(p+2) = res;
    }
    printf("const_tri(, %d) = %d\n",n , res);
}


int main()
{
    const int x = 1;
    const int *q = &x;

    printf("*(q) = x = %d\n", *q);

    int * const p = malloc(3 * sizeof(int));

    *p = x;
    *(p+1) = x;
    *(p+2) = 2 * x;


    for (int i = 0 ; i < 3 ; i++)
        printf("address cell %d -> %p\n",i , p + i );

    const_tri(p, 5);
    
    free(p);
        
    return 0;
}