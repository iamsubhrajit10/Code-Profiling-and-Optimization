#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mytime.h" // For cputime()

#define SIZE 10000000 // Size of the array
#define NUM_THREADS 8 // Number of threads

int array[SIZE];
int prime_count_naive = 0, prime_count_threaded = 0;
pthread_mutex_t mutex;

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

// Thread function for prime counting (multithreaded)
void *prime_count(void *arg)
{
    int thread_id = *(int *)arg;
    int start = (thread_id * SIZE) / NUM_THREADS;
    int end = ((thread_id + 1) * SIZE) / NUM_THREADS;
    int local_count = 0;

    for (int i = start; i < end; i++)
    {
        if (is_prime(array[i]))
        {
            local_count++;
        }
    }

    // Lock and update the global prime count
    pthread_mutex_lock(&mutex);
    prime_count_threaded += local_count;
    pthread_mutex_unlock(&mutex);

    return NULL;
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

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    prime_count_threaded = 0; // Reset global prime count for multithreading

    // Measure CPU time for multithreaded prime counting
    printf("Starting threaded prime counting...\n");
    t0 = cputime();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, prime_count, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("Threaded prime count complete. Primes found: %d\n", prime_count_threaded);
    printf("Time taken (CPU): %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup (CPU time): %Lf\n", time_naive / time_threaded);

    // Wall-clock timing using timespec
    struct timespec start, end;

    // Measure wall-clock time for naive prime counting
    printf("Starting naive prime counting (timespec)...\n");
    start = get_timespec();
    naive_prime_count();
    end = get_timespec();
    time_naive = diff_timespec_us(start, end);
    printf("Naive prime count complete. Time taken (wall-clock): %Lf microseconds\n", time_naive);

    // Reset global prime count for multithreading
    prime_count_threaded = 0;

    // Measure wall-clock time for multithreaded prime counting
    printf("Starting threaded prime counting (timespec)...\n");
    start = get_timespec();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, prime_count, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    end = get_timespec();
    time_threaded = diff_timespec_us(start, end);
    printf("Threaded prime count complete. Time taken (wall-clock): %Lf microseconds\n", time_threaded);

    // Speedup calculation for wall-clock time
    printf("Speedup (wall-clock time): %Lf\n", time_naive / time_threaded);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
