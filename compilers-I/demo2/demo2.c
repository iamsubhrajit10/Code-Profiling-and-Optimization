#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h"

int sum_of_two_arrays(int *array1, int *array2, int size)
{
    int total_sum = 0;
    for (int i = 0; i < size; i++)
    {
        int add = array1[i] + array2[i];
        total_sum += add;
    }
    return total_sum;
}

int main()
{
    long double time_taken;
    long double t0, t1;

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

    printf("Starting sum_of_two_arrays...\n");

    // Initialize time taken
    time_taken = 0;
    // Measure the CPU time taken by the sample function
    t0 = cputime();
    long long sum = 0;
    for (int i = 0; i < 10000; i++)
    {
        sum += sum_of_two_arrays(array1, array2, N);
    }
    t1 = cputime();
    time_taken = t1 - t0;

    printf("done!\n");
    printf("sum = %lld\n", sum);
    printf("Time taken: %Lf microseconds\n", time_taken);

    return 0;
}