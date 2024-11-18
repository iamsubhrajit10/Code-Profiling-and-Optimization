#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h" // For get_timespec() and diff_timespec_us()

#define SIZE 1000000000 // Size of the vectors -> 1e9 elements

void saxpy_base(float a, float *X, float *Y, int size)
{
    for (int i = 0; i < size; i++)
    {
        Y[i] = a * X[i] + Y[i];
    }
}

void saxpy_omp(float a, float *restrict X, float *restrict Y, int size)
{
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < size; i++)
    {
        Y[i] = a * X[i] + Y[i];
    }
}

int main()
{
    float a = 2.0f;
    float *X = (float *)malloc(SIZE * sizeof(float));
    float *Y_base = (float *)malloc(SIZE * sizeof(float));
    float *Y_omp = (float *)malloc(SIZE * sizeof(float));

    // Initialize vectors X and Y with random values
    for (int i = 0; i < SIZE; i++)
    {
        X[i] = (float)(rand() % 1000) / 100.0f;
        Y_base[i] = Y_omp[i] = (float)(rand() % 1000) / 100.0f;
    }

    // Measure CPU time for base case SAXPY
    printf("Starting base case SAXPY...\n");
    double start_time = omp_get_wtime();
    saxpy_base(a, X, Y_base, SIZE);
    double end_time = omp_get_wtime();
    double time_base = end_time - start_time;
    printf("Base case SAXPY completed in %f seconds.\n", time_base);

    // Measure CPU time for OpenMP SAXPY
    printf("Starting OpenMP SAXPY...\n");
    start_time = omp_get_wtime();
    saxpy_omp(a, X, Y_omp, SIZE);
    end_time = omp_get_wtime();
    double time_omp = end_time - start_time;
    printf("OpenMP SAXPY completed in %f seconds.\n", time_omp);

    // Speedup calculation
    printf("Speedup (CPU time): %f\n", time_base / time_omp);

    // Wall-clock timing using timespec
    struct timespec start, end;
    long double time_base_ts, time_omp_ts;

    // Measure wall-clock time for base case SAXPY
    printf("Starting base case SAXPY (timespec)...\n");
    start = get_timespec();
    saxpy_base(a, X, Y_base, SIZE);
    end = get_timespec();
    time_base_ts = diff_timespec_us(start, end);
    printf("Base case SAXPY completed in %Lf microseconds.\n", time_base_ts);

    // Measure wall-clock time for OpenMP SAXPY
    printf("Starting OpenMP SAXPY (timespec)...\n");
    start = get_timespec();
    saxpy_omp(a, X, Y_omp, SIZE);
    end = get_timespec();
    time_omp_ts = diff_timespec_us(start, end);
    printf("OpenMP SAXPY completed in %Lf microseconds.\n", time_omp_ts);

    // Speedup calculation for wall-clock time
    printf("Speedup (wall-clock time): %Lf\n", time_base_ts / time_omp_ts);

    free(X);
    free(Y_base);
    free(Y_omp);

    return 0;
}