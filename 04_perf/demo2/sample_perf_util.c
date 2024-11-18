
// To run the code:
// 1. Compile the code: gcc -o sample sample_perf_util.c perf_utils.c -I.
// 2. Run the code: sudo ./sample
#include <stdio.h>
#include "perf_utils.h"

int sample_function(int p)
{
    int n = 0;
    for (int i = 0; i < p; i++)
    {
        n = n + 1;
    }
    return n;
}

int main()
{
    long long values[MAX_EVENTS];

    // Initialize performance monitoring
    initialize_perf();

    // Start performance monitoring
    start_perf();

    // Your code to be profiled
    int x = sample_function(100000);
    printf("x = %d\n", x);

    // Stop performance monitoring
    stop_perf();

    // Read performance counters
    read_perf(values);

    // Write performance counters to a file or stdout
    write_perf(stdout, values);

    return 0;
}