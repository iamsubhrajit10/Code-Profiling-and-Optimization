#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h" // For cputime()
#include <unistd.h>

#define SIZE 200000000 // Size of the vectors

int A[SIZE], B[SIZE], C[SIZE]; // Vectors for single-threaded and multi-threaded additions

// Naive vector addition function (single-threaded)
void naive_vector_add()
{
    for (int i = 0; i < SIZE; i++)
    {
        C[i] = A[i] + B[i];
    }
}

// Vector addition function using OpenMP (multithreaded)
void omp_vector_add()
{
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    long double time_naive, time_threaded;
    long double t0, t1;

    // Initialize vectors A and B with random values
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
    }

    // Measure CPU time for naive vector addition
    printf("Starting naive vector addition...\n");
    t0 = cputime();
    naive_vector_add();
    t1 = cputime();
    time_naive = t1 - t0;
    printf("Naive addition complete. Time taken: %Lf microseconds\n", time_naive);

    sleep(2);

    // Measure CPU time for OpenMP vector addition
    printf("Starting OpenMP vector addition...\n");
    t0 = cputime();
    omp_vector_add();
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("OpenMP addition complete. Time taken: %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup (CPU time): %Lf\n", time_naive / time_threaded);

    // Starting measurement using timespec for wall-clock time
    struct timespec start, end;

    // Measure wall-clock time for naive vector addition
    printf("Starting naive vector addition with timespec...\n");
    start = get_timespec();
    naive_vector_add();
    end = get_timespec();
    time_naive = diff_timespec_us(start, end);
    printf("Naive addition complete. Time taken: %Lf microseconds\n", time_naive);

    // Measure wall-clock time for OpenMP vector addition
    printf("Starting OpenMP vector addition with timespec...\n");
    start = get_timespec();
    omp_vector_add();
    end = get_timespec();
    time_threaded = diff_timespec_us(start, end);
    printf("OpenMP addition complete. Time taken: %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup (wall-clock time): %Lf\n", time_naive / time_threaded);

    return 0;
}