#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // contains the limits (MAX VALUES FOR EACH DATATYPE)
#include <float.h> //  contains the limits for floating point datatype (float and double)

int main()
{
    int x = INT_MAX;
    unsigned short int y = USHRT_MAX;
    signed long int z = LONG_MAX;
    float foo = FLT_MAX;
    double bar = DBL_MAX;

    printf("DATA_TYPE: integer,   MAX VALUE: %d,   size: %lu\n\n", x, sizeof(x));
    printf("DATA_TYPE: unsigned short integer,   MAX VALUE: %hu,   size: %lu\n\n", y, sizeof(y));
    printf("DATA_TYPE: signed long int,   MAX VALUE: %ld   size: %ld\n\n", z, sizeof(z));
    printf("DATA_TYPE: float,   MAX VALUE: %f,   size: %lu\n\n", foo, sizeof(foo));
    printf("DATA_TYPE: double, MAX VALUE: %f,  size: %lu\n", bar, sizeof(bar));
    return 0;
}
