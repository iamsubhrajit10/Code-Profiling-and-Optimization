// To run the code:
// 1. Compile the code: gcc -o sample sample_perf_util.c perf_utils.c -I.
// 2. Run the code: sudo ./sample
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "perf_utils.h"

#define ARRAY_SIZE 1000000

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

void initialize_arrays(int *a, int *b, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }
}

int naive_function(int *a, int *b, int *c, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float r = (float)rand() / RAND_MAX;
        if (r < 0.01)
        {
            c[i] = a[i] * b[i];
        }
        else
        {
            c[i] = a[i] + b[i];
        }
    }
    return 0;
}

int optimized_function(int *a, int *b, int *c, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float r = (float)rand() / RAND_MAX;
        if (UNLIKELY(r < 0.01))
        {
            c[i] = a[i] * b[i];
        }
        else
        {
            c[i] = a[i] + b[i];
        }
    }
    return 0;
}

int main()
{
    srand(time(NULL));

    int *a = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *b = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *c = (int *)malloc(ARRAY_SIZE * sizeof(int));

    if (a == NULL || b == NULL || c == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initialize_arrays(a, b, ARRAY_SIZE);

    long long values[MAX_EVENTS];

    // Initialize performance monitoring
    initialize_perf();

    // Start performance monitoring for naive function
    start_perf();
    naive_function(a, b, c, ARRAY_SIZE);
    stop_perf();
    read_perf(values);
    printf("Naive function performance:\n");
    write_perf(stdout, values);

    // Start performance monitoring for optimized function
    start_perf();
    optimized_function(a, b, c, ARRAY_SIZE);
    stop_perf();
    read_perf(values);
    printf("Optimized function performance:\n");
    write_perf(stdout, values);

    free(a);
    free(b);
    free(c);

    return 0;
}