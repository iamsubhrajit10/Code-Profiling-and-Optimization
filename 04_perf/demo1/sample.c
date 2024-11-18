#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

int sum_of_two_arrays_merged_loop(int *array1, int *array2, int size)
{
    int total_sum = 0;
    for (int i = 0; i < size; i++)
    {
        total_sum += array1[i] + array2[i];
    }
    return total_sum;
}

int main()
{
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

    printf("\nStarting sum_of_two_arrays...\n");

    int x = sum_of_two_arrays(array1, array2, N);

    printf("sum_of_two_arrays: %d\n", x);

    printf("\nStarting sum_of_two_arrays_merged_loop...\n");

    int y = sum_of_two_arrays_merged_loop(array1, array2, N);

    printf("sum_of_two_arrays_merged_loop: %d\n", y);

    return 0;
}