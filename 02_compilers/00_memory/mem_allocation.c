#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mytime.h"

void allocate_in_chunk()
{

    unsigned long long start_ticks, end_ticks, ticks_taken;
    // Scenario 1: Allocate a single chunk of 1GB memory using malloc/calloc
    size_t size = 1 << 30; // 1GB in bytes

    // catch the start ticks
    start_ticks = measure_rdtsc_start();

    /******* allocate 1GB memory using malloc *******/
    char *ptr = (char *)malloc(size);
    /******* allocation ends *******/

    // catch the end ticks
    end_ticks = measure_rdtscp_end();

    ticks_taken = end_ticks - start_ticks;

    printf("Ticks taken to allocate 1GB memory using malloc: %llu\n", ticks_taken);

    free(ptr);
}

void allocate_in_units()
{
    unsigned long long start_ticks, end_ticks, ticks_taken;
    size_t size = 1 << 30; // 1GB in bytes

    // Scenario 2: Allocate 1000's of memory units summing up to 1GB
    size_t unitSize = size / 1000;
    void *units[1000];

    // catch the start ticks
    start_ticks = measure_rdtsc_start();

    /******* allocate 1000 units of memory using malloc *******/
    for (int i = 0; i < 1000; i++)
    {
        units[i] = malloc(unitSize);
    }
    /******* allocation ends *******/

    // catch the end ticks
    end_ticks = measure_rdtscp_end();

    ticks_taken = end_ticks - start_ticks;

    printf("Ticks to allocate 1000 units of %lu bytes each: %llu\n", unitSize, ticks_taken);

    for (int i = 0; i < 1000; i++)
    {
        free(units[i]);
    }
}

int main()
{
    allocate_in_chunk();
    allocate_in_units();
    return 0;
}