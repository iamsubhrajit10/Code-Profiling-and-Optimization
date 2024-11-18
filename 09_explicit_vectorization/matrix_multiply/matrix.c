#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>
#include <string.h>
#include "mytime.h"

#define N 1024 // Define the size of the matrices (N x N)

void initialize_matrix(int matrix[N][N])
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 100; // Random integer values
        }
    }
}

void transpose(const int B[N][N], int BT[N][N])
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            BT[j][i] = B[i][j];
        }
    }
}

void naive_matrix_multiply(const int A[N][N], const int B[N][N], int C[N][N])
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (size_t k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_multiply(const int A[N][N], const int B[N][N], int C[N][N])
{
    int(*BT)[N] = (int(*)[N])_mm_malloc(N * N * sizeof(int), 64);
    transpose(B, BT);
    __m512i zeros = _mm512_setzero_si512();

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            __m512i c_vec = zeros;

            // Initialize C[i][j] to 0 before starting the summation
            C[i][j] = 0;
            for (size_t k = 0; k < N; k += 16)
            {
                __m512i a_vec = _mm512_load_epi32(&A[i][k]);
                __m512i b_vec = _mm512_load_epi32(&BT[j][k]);
                c_vec = _mm512_add_epi32(c_vec, _mm512_mullo_epi32(a_vec, b_vec));
            }

            int sum = _mm512_reduce_add_epi32(c_vec);
            C[i][j] += sum;
        }
    }
    free(BT);
}

int main()
{
    srand(time(NULL)); // Seed the random number generator

    int(*A)[N] = (int(*)[N])_mm_malloc(N * N * sizeof(int), 64);
    int(*B)[N] = (int(*)[N])_mm_malloc(N * N * sizeof(int), 64);
    int(*C)[N] = (int(*)[N])_mm_malloc(N * N * sizeof(int), 64);
    int(*D)[N] = (int(*)[N])_mm_malloc(N * N * sizeof(int), 64);

    if (A == NULL || B == NULL || C == NULL || D == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initialize_matrix(A);
    initialize_matrix(B);
    memset(C, 0, N * N * sizeof(int));
    memset(D, 0, N * N * sizeof(int));

    // Starting measurement using timespec
    struct timespec start, end;

    printf("Starting naive matrix multiplication\n");
    // Measure CPU time for naive matrix multiplication
    start = get_timespec();
    naive_matrix_multiply(A, B, D);
    end = get_timespec();
    long time_taken_naive = diff_timespec_us(start, end);
    printf("Time taken (Naive): %ld microseconds\n", time_taken_naive);

    printf("Starting AVX-512 matrix multiplication\n");

    // Measure CPU time for AVX-512 matrix multiplication
    start = get_timespec();
    matrix_multiply(A, B, C);
    end = get_timespec();
    long time_taken_avx512 = diff_timespec_us(start, end);
    printf("Time taken (AVX-512): %ld microseconds\n", time_taken_avx512);

    printf("Speedup: %f\n", (double)time_taken_naive / time_taken_avx512);

    // Check correctness
    int correct = 1;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            if (C[i][j] != D[i][j])
            {
                printf("Error: C[%zu][%zu] = %d, D[%zu][%zu] = %d\n", i, j, C[i][j], i, j, D[i][j]);
                correct = 0;
                break;
            }
            // printf("C[%zu][%zu] = %d, D[%zu][%zu] = %d\n", i, j, C[i][j], i, j, D[i][j]);
        }
        if (!correct)
            break;
    }

    if (correct)
    {
        printf("The results are correct.\n");
    }
    else
    {
        printf("The results are incorrect.\n");
    }

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
            fprintf(file, "%d ", C[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(D);

    return 0;
}