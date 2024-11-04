#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mytime.h"

// static const 2D array with precomputed sums of all pairs (0-9)
static const int precomputed_sum[10][10] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
    {3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
    {5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
    {8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
    {9, 10, 11, 12, 13, 14, 15, 16, 17, 18}};

void sum_of_two_arrays(int *array1, int *array2, int *array3, int size)
{
    int total_sum = 0;
    for (int i = 0; i < size; i++)
    {
        array3[size - i - 1] += array1[i] + array2[i];
    }
}
void sum_of_two_arrays_precomputed(int *array1, int *array2, int *array3, int size)
{
    int total_sum = 0;
    for (int i = 0; i < size; i++)
    {
        array3[size - i - 1] += precomputed_sum[array1[i]][array2[i]];
    }
}
int main()
{
    long double time_taken;
    long double t0, t1;

    // define two random arrays of size N = 100000 elements
    int N = 10000000;
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
    // initialize the result array
    int *array3 = (int *)malloc(N * sizeof(int));
    memset(array3, 0, N * sizeof(int));

    printf("Starting sum_of_two_arrays usually...\n");

    // Initialize time taken
    time_taken = 0;
    // Measure the CPU time taken by the sample function
    t0 = cputime();
    long long sum = 0;
    for (int i = 0; i < 100; i++)
    {
        sum_of_two_arrays(array1, array2, array3, N);
    }
    t1 = cputime();
    time_taken = t1 - t0;

    printf("done!\n");
    printf("First 10 elements of array3: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", array3[i]);
    }
    printf("\n");
    printf("Time taken: %Lf microseconds\n", time_taken);

    memset(array3, 0, N * sizeof(int));

    printf("Starting sum_of_two_arrays_precomputed...\n");

    // Initialize time taken
    time_taken = 0;
    // Measure the CPU time taken by the sample function
    t0 = cputime();
    for (int i = 0; i < 100; i++)
    {
        sum_of_two_arrays_precomputed(array1, array2, array3, N);
    }
    t1 = cputime();
    time_taken = t1 - t0;

    printf("done!\n");
    printf("First 10 elements of array3: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", array3[i]);
    }
    printf("\n");
    printf("Time taken: %Lf microseconds\n", time_taken);

    return 0;
}