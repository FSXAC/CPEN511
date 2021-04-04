/**
 * This program is based on the work from the Institute of Applied Information Processing and Communications (IAIK)
 * available here: https://github.com/IAIK/flush_flush
 */

#include <stdio.h>
#include <string.h>

#include "util.hpp"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define PROBES_RANGE 1000
#define PROBES 1000000

size_t array[5*1024];
size_t hit_histogram[PROBES_RANGE];
size_t miss_histogram[PROBES_RANGE];

int main(void) {

    printf("=== CALIBRATION PROGRAM ===\n");

    /* Initialize performance measuring for ARM */
	#if defined(__arm__) || defined(__arm64__)
	#ifdef SKIP_PMU_ENABLE
	printf("Purposely skipped PMU enable for ARM\n")
	#else
	init();
	#endif
	#endif

    /* Set set-value of array to a bunch of -1 */
    memset(array, -1, 5 * 1024 * sizeof(size_t));

    /* First access the array to load it into cache */
    access(array + 2 * 1024);
    for (int i = 0; i < PROBES; i++)
    {
        /* Probe an address in the middle of the array */
        size_t d = probe_block((ADDR_PTR) array + 2 * 1024);
        hit_histogram[MIN(PROBES_RANGE - 1, d)]++;
    }

    /* Flush block from cache */
    flush_one_block((ADDR_PTR) array + 1024);

    /* Access data from main memory */
    for (int i = 0; i < PROBES; i++)
    {
        size_t d = probe_block((ADDR_PTR) array + 2 * 1024);
        flush_one_block((ADDR_PTR) array + 2 * 1024);
        miss_histogram[MIN(PROBES_RANGE - 1, d)]++;
    }

    /* Compute stats */
    int hit_max = 0;
    int hit_max_index = 0;
    int miss_min_index = 0;

    /* Write histogram to file */
    FILE *out_file = fopen("calibration_stats.csv", "w");

    fprintf(out_file, "Cycles,Hits,Misses\n");
    for (int i = 0; i < PROBES_RANGE; i++)
    {
        fprintf(out_file, "%d,%d,%d\n", i, hit_histogram[i], miss_histogram[i]);

        /* Find min and max cycles for hit and miss */
        if (hit_max < hit_histogram[i]) {
            hit_max = hit_histogram[i];
            hit_max_index = i;
        }

        if (miss_histogram[i] > 3 && miss_min_index == 0)
            miss_min_index = i;
    }

    int min = -1;
    int min_index = 0;
    for (int i = hit_max_index; i < miss_min_index; i++) {
        if (min > (hit_histogram[i] + miss_histogram[i])) {
            min = hit_histogram[i] + miss_histogram[i];
            min_index = i;
        }
    }
    fclose(out_file);

    printf("Calibration histogram saved in calibration_stats.csv\n");
    printf("Suggested cache hit/miss threshold: %d\n", min_index);

    return 0;
}