#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mytime.h" // For cputime()

#define SIZE 1000000         // Size of the array
#define NUM_THREADS 4        // Number of threads
#define TASK_BATCH_SIZE 1000 // Number of elements each thread processes per task

int array[SIZE];
int prime_count_naive = 0, prime_count_threaded = 0, prime_count_pool = 0;
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

// Thread function for multi-threaded prime counting
void *prime_count_threaded_func(void *arg)
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

    pthread_mutex_lock(&mutex);
    prime_count_threaded += local_count;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Task structure for the thread pool
typedef struct
{
    int start;
    int end;
} Task;

// Thread function for processing tasks in the thread pool
void *prime_count_pool_worker(void *arg)
{
    Task *task = (Task *)arg;
    int local_count = 0;

    for (int i = task->start; i < task->end; i++)
    {
        if (is_prime(array[i]))
        {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    prime_count_pool += local_count;
    pthread_mutex_unlock(&mutex);

    free(task);
    return NULL;
}

// Function to add tasks to the thread pool
void prime_count_with_thread_pool()
{
    pthread_t threads[NUM_THREADS];
    int task_count = (SIZE + TASK_BATCH_SIZE - 1) / TASK_BATCH_SIZE;
    int thread_index = 0;

    for (int i = 0; i < task_count; i++)
    {
        int start = i * TASK_BATCH_SIZE;
        int end = (start + TASK_BATCH_SIZE < SIZE) ? start + TASK_BATCH_SIZE : SIZE;

        Task *task = (Task *)malloc(sizeof(Task));
        task->start = start;
        task->end = end;

        pthread_create(&threads[thread_index], NULL, prime_count_pool_worker, (void *)task);
        thread_index = (thread_index + 1) % NUM_THREADS;

        // Wait for threads to complete if all are busy
        if (thread_index == 0 || i == task_count - 1)
        {
            for (int j = 0; j < NUM_THREADS && j <= i; j++)
            {
                pthread_join(threads[j], NULL);
            }
        }
    }
}

int main()
{
    long double time_naive, time_threaded, time_pool;
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

    // Multi-threaded prime counting
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    prime_count_threaded = 0;

    printf("Starting threaded prime counting...\n");
    t0 = cputime();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, prime_count_threaded_func, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    t1 = cputime();
    time_threaded = t1 - t0;
    printf("Threaded prime count complete. Primes found: %d\n", prime_count_threaded);
    printf("Time taken (CPU): %Lf microseconds\n", time_threaded);

    printf("Speedup (CPU time): %Lf\n", time_naive / time_threaded);

    // Thread pool prime counting
    prime_count_pool = 0;
    printf("Starting prime counting with thread pool...\n");
    t0 = cputime();
    prime_count_with_thread_pool();
    t1 = cputime();
    time_pool = t1 - t0;
    printf("Thread pool prime count complete. Primes found: %d\n", prime_count_pool);
    printf("Time taken (CPU): %Lf microseconds\n", time_pool);

    printf("Speedup with thread pool: %Lf\n", time_naive / time_pool);

    pthread_mutex_destroy(&mutex);

    return 0;
}
