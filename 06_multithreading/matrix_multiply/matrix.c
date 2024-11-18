#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h" // For cputime()

#define SIZE 2000     // Size of the matrix
#define NUM_THREADS 4 // Number of threads

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE], D[SIZE][SIZE];

// Naive matrix multiplication function (single-threaded)
void naive_matrix_multiply()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            D[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
            {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Thread function for matrix multiplication (multithreaded)
void *multiply(void *arg)
{
    int thread_id = *(int *)arg;
    int start_row = (thread_id * SIZE) / NUM_THREADS;
    int end_row = ((thread_id + 1) * SIZE) / NUM_THREADS;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

int main()
{
    long double time_naive, time_threaded;
    long double t0, t1;

    // Initialize matrices A and B with random values
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            A[i][j] = B[i][j] = rand() % 10;
        }
    }

    // Measure CPU time for naive matrix multiplication
    printf("Starting naive matrix multiplication...\n");
    t0 = cputime();
    naive_matrix_multiply();
    t1 = cputime();
    time_naive = t1 - t0;
    printf("Naive multiplication complete. Time taken: %Lf microseconds\n", time_naive);

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Measure CPU time for multithreaded matrix multiplication
    printf("Starting threaded matrix multiplication...\n");
    t0 = cputime();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("Threaded multiplication complete. Time taken: %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup: %Lf\n", time_naive / time_threaded);

    // Starting measurement using timespec
    struct timespec start, end;

    // Measure CPU time for naive matrix multiplication
    printf("Starting naive matrix multiplication...\n");
    start = get_timespec();
    naive_matrix_multiply();
    end = get_timespec();
    time_naive = diff_timespec_us(start, end);

    printf("Naive multiplication complete. Time taken: %Lf microseconds\n", time_naive);

    // pthread_t threads[NUM_THREADS];
    // int thread_ids[NUM_THREADS];

    // Measure CPU time for multithreaded matrix multiplication
    printf("Starting threaded matrix multiplication...\n");
    start = get_timespec();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    end = get_timespec();
    time_threaded = diff_timespec_us(start, end);
    printf("Threaded multiplication complete. Time taken: %Lf microseconds\n", time_threaded);

    // Speedup calculation
    printf("Speedup: %Lf\n", time_naive / time_threaded);

    return 0;
}
