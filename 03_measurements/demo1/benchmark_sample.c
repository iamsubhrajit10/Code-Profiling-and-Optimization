#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h"

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

int main()
{
    long double time_taken;
    struct timespec start, end;
    long double t0, t1;
    unsigned long niter;
    double ops_per_sec, f;
    int decimals;

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

    // Initialize time taken
    time_taken = 0;

    // Measure the CPU time taken by the sample function
    printf("Starting sum_of_two_arrays...\n");
    printf("--------------------------\n");
    printf("Calibrating time using rusage...\n");

    TIME_RUSAGE(time_taken, sum_of_two_arrays(array1, array2, N));

    printf("done!\n");
    printf("Calibrated time: %Lf microseconds\n", time_taken);
    niter = 1 + (unsigned long)(1e7 / time_taken);
    printf("Performing %lu times\n", niter);
    fflush(stdout);

    t0 = cputime();
    for (int i = 0; i < niter; i++)
    {
        sum_of_two_arrays(array1, array2, N);
    }
    t1 = cputime() - t0;
    printf("done!\n");

    // Convert t1 from microseconds to seconds for the ops_per_sec calculation
    ops_per_sec = (1e6 * niter) / t1;
    f = 100.0;

    for (decimals = 0;; decimals++)
    {
        if (ops_per_sec > f)
            break;
        f = f * 0.1;
    }

    printf("RESULT: %.*f operations per second\n", decimals, ops_per_sec);

    printf("--------------------------\n");
    printf("Calibrating time using clock_gettime (timespec)...\n");

    // Initialize time taken
    time_taken = 0;
    TIME_TIMESPEC(time_taken, sum_of_two_arrays(array1, array2, N));

    printf("done!\n");
    printf("Calibrated time: %Lf microseconds\n", time_taken);
    niter = 1 + (unsigned long)(1e7 / time_taken);
    printf("Peforming %d times\n", niter);
    fflush(stdout);

    start = get_timespec();
    for (int i = 0; i < niter; i++)
    {
        sum_of_two_arrays(array1, array2, N);
    }
    end = get_timespec();
    time_taken = diff_timespec_us(start, end);

    // Convert t1 from microseconds to seconds for the ops_per_sec calculation
    ops_per_sec = (1e6 * niter) / t1;
    f = 100.0;

    for (decimals = 0;; decimals++)
    {
        if (ops_per_sec > f)
            break;
        f = f * 0.1;
    }

    printf("RESULT: %.*f operations per second\n", decimals, ops_per_sec);

    printf("--------------------------\n");
    printf("Calibrating time using rdtsc...\n");

    // Initialize time taken
    time_taken = 0;
    TIME_RDTSC(time_taken, sum_of_two_arrays(array1, array2, N));

    printf("done!\n");
    printf("Calibrated time: %Lf microseconds\n", time_taken);
    niter = 1 + (unsigned long)(1e7 / time_taken);

    printf("Peforming %d times\n", niter);
    fflush(stdout);

    unsigned long long start_ticks, end_ticks, ticks_taken;
    start_ticks = measure_rdtsc_start();
    for (int i = 0; i < niter; i++)
    {
        sum_of_two_arrays(array1, array2, N);
    }
    end_ticks = measure_rdtscp_end();

    ticks_taken = end_ticks - start_ticks;

    time_taken = ticks_taken * 0.000357;

    ops_per_sec = (1e6 * niter) / time_taken;
    f = 100.0;

    for (decimals = 0;; decimals++)
    {
        if (ops_per_sec > f)
            break;
        f = f * 0.1;
    }

    printf("RESULT: %.*f operations per second\n", decimals, ops_per_sec);

    return 0;
}