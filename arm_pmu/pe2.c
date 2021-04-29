#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>

int main(int argc, char **argv) {
    /* File descriptor used to read mispredicted branches counter. */
    static int perf_fd_branch_miss;
    
    /* Initialize our perf_event_attr, representing one counter to be read. */
    static struct perf_event_attr attr_branch_miss;
    attr_branch_miss.size = sizeof(attr_branch_miss);
    attr_branch_miss.exclude_kernel = 1;
    attr_branch_miss.exclude_hv = 1;
    attr_branch_miss.exclude_callchain_kernel = 1;

    #ifdef REAL_SYS
    /* On a real system, you can do like this: */
    attr_branch_miss.type = PERF_TYPE_HARDWARE;
    attr_branch_miss.config = PERF_COUNT_HW_BRANCH_MISSES;
    #else
    /* On a gem5 system, you have to do like this: */
    attr_branch_miss.type = PERF_TYPE_RAW;
    attr_branch_miss.config = 0x10;
    #endif
    
    /* Open the file descriptor corresponding to this counter. The counter
       should start at this moment. */
    if ((perf_fd_branch_miss = syscall(__NR_perf_event_open, &attr_branch_miss, 0, -1, -1, 0)) == -1)
        fprintf(stderr, "perf_event_open fail %d %d: %s\n", perf_fd_branch_miss, errno, strerror(errno));
    
    /* Workload here, that means our specific task to profile. */

    /* Get and close the performance counters. */
    uint64_t counter_branch_miss = 0;
    read(perf_fd_branch_miss, &counter_branch_miss, sizeof(counter_branch_miss));
    close(perf_fd_branch_miss);

    /* Display the result. */
    printf("Number of mispredicted branches: %d\n", counter_branch_miss);
}