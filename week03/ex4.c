#include <stdio.h>
#include <stdlib.h>

void *addInt(const void *a, const void *b)
{
    int *res = malloc(sizeof(int));
    *res = *((int *)a) + *((int *)b);
    return res;
}

void *addDouble(const void *a, const void *b)
{
    double *res = malloc(sizeof(double));
    *res = *((double *)a) + *((double *)b);
    return res;
}

void *mulInt(const void *a, const void *b)
{
    int *res = malloc(sizeof(int));
    *res = *((int *)a) * *((int *)b);
    return res;
}

void *mulDouble(const void *a, const void *b)
{
    double *res = malloc(sizeof(double));
    *res = *((double *)a) * *((double *)b);
    return res;
}

void *meanInt(const void *a, const void *b)
{
    int *res = malloc(sizeof(int));
    *res = *((int *)a) + *((int *)b);
    *res /= 2;
    return res;
}

void *meanDouble(const void *a, const void *b)
{
    double *res = malloc(sizeof(double));
    *res = *((double *)a) + *((double *)b);
    *res /= 2;
    return res;
}

void *aggregate(void *base, size_t size, int n, void *initial_value, void *(*opr)(const void *, const void *))
{
    void *output;
    if (size == sizeof(int))
    { // base is a pointer to an integer

        int res = *(int *)initial_value;
        int *a = (int *)base;
        for (int i = 0; i < n; i++)
        {
            int *tmp = opr(&res, a + i);
            res = *tmp;
            free(tmp);
        }
        if (opr == meanInt)
        {
            res /= n;
        }
        int *tmp = malloc(sizeof(int));
        *tmp = res;
        output = tmp;
    }
    else
    { // base is a pointer to a double

        double res = *(double *)initial_value;
        double *a = (double *)base;
        for (int i = 0; i < n; i++)
        {
            double *tmp = opr(&res, a + i);
            res = *tmp;
            free(tmp);
        }
        if (opr == meanDouble)
        {
            res /= n;
        }
        double *tmp = malloc(sizeof(double));
        *tmp = res;
        output = tmp;
    }
    return output;
}

int main()
{

    int *ints = malloc(sizeof(int) * 5);
    double *doubles = malloc(sizeof(double) * 5);

    for (int i = 0; i < 5; i++)
        ints[i] = i + 5, doubles[i] = (double)(i + 10) * 2 / 3.14;

    int initial_value_int;
    double initial_value_double;

    // Addition

    int *result1a;

    initial_value_int = 0;
    result1a = aggregate(ints, sizeof(int), 5, &initial_value_int, addInt);

    printf("%d\n", *result1a);

    double *result2a;

    initial_value_double = 0.0;
    result2a = aggregate(doubles, sizeof(double), 5, &initial_value_double, addDouble);

    printf("%f\n", *result2a);

    // Multiplication

    int *result1m;

    initial_value_int = 1;
    result1m = aggregate(ints, sizeof(int), 5, &initial_value_int, mulInt);

    printf("%d\n", *result1m);

    double *result2m;

    initial_value_double = 1.0;
    result2m = aggregate(doubles, sizeof(double), 5, &initial_value_double, mulDouble);

    printf("%f\n", *result2m);

    // Mean

    int *result1mean;

    initial_value_int = 0;
    result1mean = aggregate(ints, sizeof(int), 5, &initial_value_int, meanInt);

    printf("%d\n", *result1mean);

    double *result2mean;

    initial_value_double = 0.0;
    result2mean = aggregate(doubles, sizeof(double), 5, &initial_value_double, meanDouble);

    printf("%f\n", *result2mean);

    // free the pointers
    free(ints);
    free(doubles);
    free(result1a);
    free(result1m);
    free(result1mean);
    free(result2a);
    free(result2m);
    free(result2mean);

    // for safer measurements
    ints = NULL;
    doubles = NULL;
    result1a = NULL;
    result1m = NULL;
    result1mean = NULL;
    result2a = NULL;
    result2m = NULL;
    result2mean = NULL;

    return EXIT_SUCCESS;
}