#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include "mytime.h"

#define N 1024 // Define the size of the matrices (N x N)

void initialize_matrix(int *matrix, size_t n)
{
    matrix = (int *)__builtin_assume_aligned(matrix, 64);
    for (size_t i = 0; i < n * n; i++)
    {
        matrix[i] = rand() % 100; // Random integer values
    }
}

void matrix_multiply(int *A, int *B, int *C, size_t n)
{
    A = (int *)__builtin_assume_aligned(A, 64);
    B = (int *)__builtin_assume_aligned(B, 64);
    C = (int *)__builtin_assume_aligned(C, 64);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            C[i * n + j] = 0;
            for (size_t k = 0; k < n; k++)
            {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

int main()
{
    srand(time(NULL)); // Seed the random number generator

    int *A = (int *)_mm_malloc(N * N * sizeof(int), 64);
    int *B = (int *)_mm_malloc(N * N * sizeof(int), 64);
    int *C = (int *)_mm_malloc(N * N * sizeof(int), 64);

    if (A == NULL || B == NULL || C == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initialize_matrix(A, N);
    initialize_matrix(B, N);

    // Starting measurement using timespec
    struct timespec start, end;

    // Measure CPU time
    start = get_timespec();

    matrix_multiply(A, B, C, N);

    end = get_timespec();

    long time_taken = diff_timespec_us(start, end);

    printf("Time taken: %ld microseconds\n", time_taken);

    // Write the result matrix to a file
    FILE *file = fopen("matrix_output.txt", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    fprintf(file, "Matrix C (A * B):\n");
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            fprintf(file, "%d ", C[i * N + j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    return 0;
}