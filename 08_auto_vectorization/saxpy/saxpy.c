#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include "mytime.h"

#define LEN 10000000
void initialize_array(int *array, size_t len)
{
    array = (int *)__builtin_assume_aligned(array, 64);
    for (size_t i = 0; i < len; i++)
    {
        array[i] = rand() % 100; // Random integer values
    }
}

void saxpy(int a, int *X, int *Y, size_t len)
{
    X = (int *)__builtin_assume_aligned(X, 64);
    Y = (int *)__builtin_assume_aligned(Y, 64);

    for (size_t i = 0; i < len; i++)
    {
        Y[i] = a * X[i] + Y[i];
    }
}

int main()
{
    srand(time(NULL)); // Seed the random number generator

    int *X = (int *)_mm_malloc(LEN * sizeof(int), 64);
    int *Y = (int *)_mm_malloc(LEN * sizeof(int), 64);
    int a = rand() % 100; // Random integer scalar

    if (X == NULL || Y == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initialize_array(X, LEN);
    initialize_array(Y, LEN);

    // Starting measurement using timespec
    struct timespec start, end;

    // Measure CPU time
    start = get_timespec();

    saxpy(a, X, Y, LEN);

    end = get_timespec();

    long time_taken = diff_timespec_us(start, end);

    printf("Time taken: %ld microseconds\n", time_taken);

    // Write the result array to a file
    FILE *file = fopen("saxpy_output.txt", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    fprintf(file, "Array Y (a * X + Y):\n");
    for (size_t i = 0; i < LEN; i++)
    {
        fprintf(file, "%d ", Y[i]);
    }
    fprintf(file, "\n");

    fclose(file);

    _mm_free(X);
    _mm_free(Y);

    return 0;
}