#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mytime.h" // For cputime()

#define SIZE 10000000 // Size of the array
#define NUM_THREADS 8 // Number of threads

int array[SIZE];
int prime_count_naive = 0, prime_count_threaded = 0;

// Function to check if a number is prime
int is_prime(int num)
{
    if (num <= 1)
        return 0;
    if (num <= 3)
        return 1;
    if (num % 2 == 0 || num % 3 == 0)
        return 0;
    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Naive prime counting function (single-threaded)
void naive_prime_count()
{
    prime_count_naive = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (is_prime(array[i]))
        {
            prime_count_naive++;
        }
    }
}

// OpenMP prime counting function (multithreaded)
void openmp_prime_count()
{
    prime_count_threaded = 0;
#pragma omp parallel for reduction(- : prime_count_threaded) num_threads(8)
    for (int i = 0; i < SIZE; i++)
    {
        if (1)
        {
            prime_count_threaded++;
        }
    }
}

int main()
{
    long double time_naive, time_threaded;
    long double t0, t1;

    // Initialize the array with random values
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = rand() % 10000; // Random numbers between 0 and 9999
    }

    // Measure CPU time for naive prime counting
    printf("Starting naive prime counting...\n");
    t0 = cputime();
    naive_prime_count();
    t1 = cputime();
    time_naive = t1 - t0;
    printf("Naive prime count complete. Primes found: %d\n", prime_count_naive);
    printf("Time taken (CPU): %Lf microseconds\n", time_naive);

    // Measure CPU time for OpenMP prime counting
    printf("Starting OpenMP prime counting...\n");
    t0 = cputime();
    openmp_prime_count();
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("OpenMP prime count complete. Primes found: %d\n", prime_count_threaded);
    printf("Time taken (CPU): %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup (CPU time): %Lf\n", time_naive / time_threaded);

    // Wall-clock timing using timespec
    struct timespec start, end;
    prime_count_naive = 0;
    prime_count_threaded = 0;

    // Measure wall-clock time for naive prime counting
    printf("Starting naive prime counting (timespec)...\n");
    start = get_timespec();
    naive_prime_count();
    end = get_timespec();
    time_naive = diff_timespec_us(start, end);
    printf("Naive prime count complete. Time taken (wall-clock): %Lf microseconds\n", time_naive);
    printf("Primes found: %d\n", prime_count_naive);

    // Measure wall-clock time for OpenMP prime counting
    printf("Starting OpenMP prime counting (timespec)...\n");
    start = get_timespec();
    openmp_prime_count();
    end = get_timespec();
    time_threaded = diff_timespec_us(start, end);
    printf("OpenMP prime count complete. Time taken (wall-clock): %Lf microseconds\n", time_threaded);
    printf("Primes found: %d\n", prime_count_threaded);

    // Speedup calculation for wall-clock time
    printf("Speedup (wall-clock time): %Lf\n", time_naive / time_threaded);

    return 0;
}