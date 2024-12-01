#ifndef MYTIME_H
#define MYTIME_H

#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
// Function to measure CPU time in microseconds as a long double
static inline long double cputime()
{
    struct rusage rus;
    getrusage(RUSAGE_SELF, &rus);
    return rus.ru_utime.tv_sec * 1000000.0L + rus.ru_utime.tv_usec;
}

// function to get the timespec stamp
static inline struct timespec get_timespec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts;
}

// function to compute the difference between two timespec stamps in microseconds
static inline long diff_timespec_us(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    // return in microseconds
    return (temp.tv_sec * 1000000000 + temp.tv_nsec) / 1000;
}

static inline unsigned long long measure_rdtsc_start()
{
    unsigned cycles_low, cycles_high;
    unsigned long long ticks;
    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t" : "=r"(cycles_high), "=r"(cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
    ticks = (((unsigned long long)cycles_high << 32) | cycles_low);
    return ticks;
}
// inline function for measuring rdstcp ticks
static inline unsigned long long measure_rdtscp_end()
{
    unsigned cycles_low, cycles_high;
    unsigned long long ticks;
    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t" : "=r"(cycles_high), "=r"(cycles_low)::"%rax",
                               "%rbx", "%rcx", "%rdx");
    ticks = (((unsigned long long)cycles_high << 32) | cycles_low);
    return ticks;
}

// Function to measure the time taken by a function using the rusage system call
#define TIME_RUSAGE(t, func)                    \
    do                                          \
    {                                           \
        long int __t0, __times, __t, __tmp;     \
        __times = 1;                            \
        {                                       \
            func;                               \
        }                                       \
        do                                      \
        {                                       \
            __times <<= 1;                      \
            __t0 = cputime();                   \
            for (__t = 0; __t < __times; __t++) \
            {                                   \
                func;                           \
            }                                   \
            __tmp = cputime() - __t0;           \
        } while (__tmp < 250000);               \
        (t) = (double)__tmp / __times;          \
    } while (0)

// Function to measure the time taken by a function using the timespec clock_gettime system call
#define TIME_TIMESPEC(t, func)                      \
    do                                              \
    {                                               \
        long int __tmp, __times;                    \
        struct timespec __t0, __t1;                 \
        __times = 1;                                \
        {                                           \
            func;                                   \
        }                                           \
        do                                          \
        {                                           \
            __times <<= 1;                          \
            __t0 = get_timespec();                  \
            for (int __t = 0; __t < __times; __t++) \
            {                                       \
                func;                               \
            }                                       \
            __t1 = get_timespec();                  \
            __tmp = diff_timespec_us(__t0, __t1);   \
        } while (__tmp < 250000);                   \
        (t) = (double)__tmp / __times;              \
    } while (0)

// Function to measure the time taken by a function using the rdtsc instruction
#define TIME_RDTSC(t, func)                            \
    do                                                 \
    {                                                  \
        unsigned long long __t0, __t1, __times, __tmp; \
        __times = 1;                                   \
        {                                              \
            func;                                      \
        }                                              \
        do                                             \
        {                                              \
            __times <<= 1;                             \
            __t0 = measure_rdtsc_start();              \
            for (int __t = 0; __t < __times; __t++)    \
            {                                          \
                func;                                  \
            }                                          \
            __t1 = measure_rdtscp_end();               \
            __tmp = __t1 - __t0;                       \
        } while (__tmp < 700000000);                   \
        __tmp = __tmp * 0.000357;                      \
        (t) = (double)(__tmp) / __times;               \
    } while (0)

#endif // MYTIME_H