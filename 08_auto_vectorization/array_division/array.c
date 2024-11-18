#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include "mytime.h"

#define LEN 100000000 // Length of the arrays -> 100 million

void initialize_array(float *array, size_t len)
{
    array = (float *)__builtin_assume_aligned(array, 64);
    for (size_t i = 0; i < len; i++)
    {
        array[i] = (float)(rand() % 1000000) / 1000.0f; // Random float values
    }
}

void compute_division(float *A, float *B, float *C, size_t len)
{
    A = (float *)__builtin_assume_aligned(A, 64);
    B = (float *)__builtin_assume_aligned(B, 64);
    C = (float *)__builtin_assume_aligned(C, 64);

    size_t i;
    for (i = 0; i < len; i++)
    {
        C[i] = A[i] / B[i];
    }
}

int main()
{
    srand(time(NULL)); // Seed the random number generator

    float *A = (float *)_mm_malloc(LEN * sizeof(float), 64);
    float *B = (float *)_mm_malloc(LEN * sizeof(float), 64);
    float *C = (float *)_mm_malloc(LEN * sizeof(float), 64);

    if (A == NULL || B == NULL || C == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initialize_array(A, LEN);
    initialize_array(B, LEN);

    // Starting measurement using timespec
    struct timespec start, end;

    // Measure CPU time
    start = get_timespec();

    compute_division(A, B, C, LEN);

    end = get_timespec();

    long time_taken = diff_timespec_us(start, end);

    printf("Time taken: %ld microseconds\n", time_taken);

    // Write the arrays to a file
    FILE *file = fopen("arrays_output.txt", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    fprintf(file, "Array C (A / B):\n");
    for (size_t i = 0; i < LEN; i++)
    {
        fprintf(file, "%f ", C[i]);
    }
    fprintf(file, "\n");

    fclose(file);

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    return 0;
}