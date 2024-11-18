
// To run the code:
// 1. Compile the code: gcc -o sample2 sample_perf_util2.c perf_utils.c -I.
// 2. Run the code: sudo ./sample
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "perf_utils.h"

int sum_of_two_arrays(int *array1, int *array2, int size)
{
    int total_sum = 0;
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < size; i++)
    {
        sum1 += array1[i];
    }
    for (int i = 0; i < size; i++)
    {
        sum2 += array2[i];
    }

    total_sum = sum1 + sum2;
    return total_sum;
}

int sum_of_two_arrays_merged_loop(int *array1, int *array2, int size)
{
    int total_sum = 0;
    for (int i = 0; i < size; i++)
    {
        total_sum += array1[i] + array2[i];
    }
    return total_sum;
}

int main()
{
    long long values[MAX_EVENTS];

    // Initialize performance monitoring
    initialize_perf();

    // define two random arrays of size N = 100000 elements
    int N = 100000;
    int *array1 = (int *)malloc(N * sizeof(int));
    int *array2 = (int *)malloc(N * sizeof(int));
    // seed the random number generator
    srand(time(NULL));
    // Initialize the arrays randomly
    for (int i = 0; i < N; i++)
    {
        array1[i] = rand() % 100;
    }

    // Initialize the arrays randomly
    for (int i = 0; i < N; i++)
    {
        array2[i] = rand() % 100;
    }

    printf("\nStarting sum_of_two_arrays...\n");

    // Start performance monitoring
    start_perf();

    // Your code to be profiled
    int x = sum_of_two_arrays(array1, array2, N);

    // Stop performance monitoring
    stop_perf();

    printf("sum = %d\n", x);

    // Read performance counters
    read_perf(values);

    // Write performance counters to a file or stdout
    write_perf(stdout, values);

    printf("\nStarting sum_of_two_arrays_merged_loop...\n");

    // Start performance monitoring
    start_perf();

    // Your code to be profiled
    // x = sum_of_two_arrays_merged_loop(array1, array2, N);

    // Stop performance monitoring
    stop_perf();

    printf("sum = %d\n", x);

    // Read performance counters
    read_perf(values);

    // Write performance counters to a file or stdout
    write_perf(stdout, values);

    return 0;
}