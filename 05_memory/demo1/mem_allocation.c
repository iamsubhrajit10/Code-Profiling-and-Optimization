
// To run the code:
// 1. Compile the code: gcc -o sample sample_perf_util.c perf_utils.c -I.
// 2. Run the code: sudo ./sample
// Commands to run
// gcc -g mem_allocation.c perf_utils.c -I..
// sudo ./a.out

// using valgrind
// sudo valgrind --tool=memcheck --leak-check=full ./a.out
// using cache grind
// sudo valgrind --tool=cachegrind ./a.out
// cg_annotate cachegrind.out.1234 > analysis.txt

#include <stdio.h>
#include <stdlib.h>
#include "perf_utils.h"

void access_value(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        int r = array[i];
    }
}

void *allocate_memory(int size)
{
    // void *array = malloc(size);
    // memset(array, 0, size);

    void *array = calloc(size, 1);
    // memset(array, 0, size);

    if (array == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    // access_value(array, size / sizeof(int));
    return array;
}

int main()
{
    long long values[MAX_EVENTS];

    // Initialize performance monitoring
    // initialize_perf();

    int N = (1 << 20); // size in bytes: 1GB
    int *array = (int *)allocate_memory(N);
    int size = N / sizeof(int);

    // Start performance monitoring
    // start_perf();

    // Your code to be profiled
    access_value(array, size);

    // Stop performance monitoring
    // stop_perf();

    // Read performance counters
    // read_perf(values);

    // Write performance counters to a file or stdout
    // write_perf(stdout, values);
    // print pid
    // printf("PID: %d\n", getpid());

    // getchar();

    array[size - 1] = 0;
    free(array);

    return 0;
}