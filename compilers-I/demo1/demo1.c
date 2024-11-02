#include <stdio.h>
#include "mytime.h"

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
    long double time_taken;
    long double t0, t1;

    // Initialize time taken
    time_taken = 0;

    // Measure the CPU time taken by the sample function
    long long sum = 0;
    t0 = cputime();
    for (int i = 0; i < 200000; i++)
    {
        sum += sample_function(i);
    }
    t1 = cputime();
    time_taken = t1 - t0;
    printf("done!\n");
    printf("sum = %lld\n", sum);
    printf("Time taken: %Lf micro seconds\n", time_taken);

    return 0;
}