#include "perf_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <errno.h>

#define MAX_EVENTS 6
#define CORE_NO -1

struct perf_event_attr pe[MAX_EVENTS];
int fd[MAX_EVENTS];
const char *event_names[MAX_EVENTS] = {
    "CPU_CYCLES",    // CPU Cycles
    "USER_INSNS",    // User Instructions
    "KERN_INSNS",    // Kernel Instructions
    "BRANCH_MISSES", // Branch Misses
    "L1D_READS",     // L1D Cache Reads
    "L1D_MISSES"     // L1D Cache Read Misses
};

void initialize_perf()
{
    // Define the events to monitor
    memset(pe, 0, sizeof(struct perf_event_attr) * MAX_EVENTS);
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        pe[i].size = sizeof(struct perf_event_attr);
        pe[i].disabled = 1;
        pe[i].exclude_kernel = 0;
        pe[i].exclude_hv = 1;
        pe[i].exclude_idle = 1;
        pe[i].exclude_user = 0; // Initialize exclude_user explicitly
        pe[i].pinned = 1;       // Ensure counter stays on the specific CPU
    }

    // CPU cycles
    pe[0].type = PERF_TYPE_HARDWARE;
    pe[0].config = PERF_COUNT_HW_CPU_CYCLES;

    // User-level instructions
    pe[1].type = PERF_TYPE_HARDWARE;
    pe[1].config = PERF_COUNT_HW_INSTRUCTIONS;
    pe[1].exclude_kernel = 1;
    pe[1].exclude_user = 0;

    // Kernel-level instructions
    pe[2].type = PERF_TYPE_HARDWARE;
    pe[2].config = PERF_COUNT_HW_INSTRUCTIONS;
    pe[2].exclude_kernel = 0;
    pe[2].exclude_user = 1;

    // Branch misses
    pe[3].type = PERF_TYPE_HARDWARE;
    pe[3].config = PERF_COUNT_HW_BRANCH_MISSES;

    // L1D Cache Reads
    pe[4].type = PERF_TYPE_HW_CACHE;
    pe[4].config = PERF_COUNT_HW_CACHE_L1D | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16);

    // L1D Cache Read Misses
    pe[5].type = PERF_TYPE_HW_CACHE;
    pe[5].config = PERF_COUNT_HW_CACHE_L1D | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);

    // Open the events
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        fd[i] = perf_event_open(&pe[i], 0, CORE_NO, -1, 0);
        if (fd[i] == -1)
        {
            fprintf(stderr, "Error opening event %d: %s\n", i, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

void read_perf(long long values[])
{
    for (int j = 0; j < MAX_EVENTS; j++)
    {
        if (read(fd[j], &values[j], sizeof(uint64_t)) == -1)
        {
            perror("Error reading counter value");
            exit(EXIT_FAILURE);
        }
    }
}

void write_perf(FILE *file, long long values[])
{
    // write the header names
    for (int j = 0; j < MAX_EVENTS; j++)
    {
        fprintf(file, "%s,", event_names[j]);
    }
    fprintf(file, "\n");
    for (int j = 0; j < MAX_EVENTS; j++)
    {
        fprintf(file, "%llu,", values[j]);
    }
    fprintf(file, "\n");
    // close the file
    // fclose(file);
}

void start_perf()
{
    for (int j = 0; j < MAX_EVENTS; j++)
    {
        ioctl(fd[j], PERF_EVENT_IOC_RESET, 0);
        ioctl(fd[j], PERF_EVENT_IOC_ENABLE, 0);
    }
}

void stop_perf()
{
    for (int j = 0; j < MAX_EVENTS; j++)
    {
        if (ioctl(fd[j], PERF_EVENT_IOC_DISABLE, 0) == -1)
        {
            perror("Error disabling counter");
            exit(EXIT_FAILURE);
        }
    }
}