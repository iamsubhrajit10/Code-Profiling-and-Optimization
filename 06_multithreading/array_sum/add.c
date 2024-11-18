#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h" // For cputime()
#include <unistd.h>

#define SIZE 200000000   // Size of the vectors
#define NUM_THREADS 1000 // Number of threads

int A[SIZE], B[SIZE], C[SIZE]; // Vectors for single-threaded and multi-threaded additions

// Naive vector addition function (single-threaded)
void naive_vector_add()
{
    for (int i = 0; i < SIZE; i++)
    {
        C[i] = A[i] + B[i];
    }
}

// Thread function for vector addition (multithreaded)
void *vector_add(void *arg)
{
    int thread_id = *(int *)arg;
    int start = (thread_id * SIZE) / NUM_THREADS;
    int end = ((thread_id + 1) * SIZE) / NUM_THREADS;

    // printf("thread id: %d, start: %d, end: %d\n", thread_id, start, end);
    for (int i = start; i < end; i++)
    {
        C[i] = A[i] + B[i];
    }
    return NULL;
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

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    sleep(2);

    // Measure CPU time for multithreaded vector addition
    printf("Starting threaded vector addition...\n");
    t0 = cputime();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, vector_add, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("Threaded addition complete. Time taken: %Lf microseconds\n", time_threaded);

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

    // Measure wall-clock time for multithreaded vector addition
    printf("Starting threaded vector addition with timespec...\n");
    start = get_timespec();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, vector_add, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    end = get_timespec();
    time_threaded = diff_timespec_us(start, end);
    printf("Threaded addition complete. Time taken: %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup (wall-clock time): %Lf\n", time_naive / time_threaded);

    return 0;
}
