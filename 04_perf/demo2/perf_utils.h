#ifndef PERF_UTILS_H
#define PERF_UTILS_H

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

extern struct perf_event_attr pe[MAX_EVENTS];
extern int fd[MAX_EVENTS];
extern long long count;
extern const char *event_names[MAX_EVENTS];

// Function declarations
void initialize_perf();
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags);
void read_perf(long long values[]);
void write_perf(FILE *file, long long values[]);
void start_perf();
void stop_perf();

#endif // PERF_UTILS_H