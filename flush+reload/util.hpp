#ifndef UTIL_H_
#define UTIL_H_

// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "padding.hpp"

#define ADDR_PTR uint64_t
#define CYCLES volatile uint64_t

/* How many bytes are in a cache line */
#define CACHE_LINE_SIZE 64

/* Time quantum for a single  */
#define TIME_SLOT 2500


#define SLOT_WINDOW_COUNT 10
#define SLOT_HIT_CUTOFF 2
#define SENDER_PULSE 4

/* Channel bit width (how many bits to send concurrently) */
#define BIT_WIDTH 9

/* The addresses for each bit in the channel */
#define ADDR_0 0x1176
#define ADDR_1 0x22fd
#define ADDR_2 0x3484
#define ADDR_3 0x460b
#define ADDR_4 0x5792
#define ADDR_5 0x6919
#define ADDR_6 0x7aa0
#define ADDR_7 0x8c27
#define ADDR_8 0x9dae

void init(void)
{
	#if defined(__arm__)
	/**
	 * http://qcd.phys.cmu.edu/QCDcluster/intel/vtune/reference/INST_MRC.htm 
	 * https://stackoverflow.com/questions/12179872/what-does-mrc-p15-do-in-arm-inline-assembly-and-how-does-gnu-c-inline-asm-synta
	 * 
	 * Use `MRC` instruction to move data from coprocessor to core registers
	 * 
	 * MRC[condition] cpname, cpopcode1, dest, cpsource, cpreg[, cpopcode2]
	 * 
	 * condition 	
	 * - One of 16 conditions. Refer to Condition Code Status.
	 * cpname		- name of the coprocessor (p0..p15)
	 * cpopcode1	- coprocessor cpname operation
	 * dest			- destination register
	 * cpsource		- source coprocessor register
	 * cpreg		- additional coprocessor register
	 * cpopcode2	- coprocessor cpname operation
	 */


	/**
	 * https://stackoverflow.com/questions/3247373/how-to-measure-program-execution-time-in-arm-cortex-a8-processor
	 * 
	 * Use MRC to control the CONTROL-COPROCESSOR (p15)
	 *
	 */

	/* Enable user mode access to performance counters */
	asm volatile ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r" (1));

	/* Disable counter interrupts */
	asm volatile ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r" (0x8000000f));

	/**
	 * Write to PMCR
	 * 
	 * [0] - Enable all counters including CCNT
	 * [1] - Reset all performance counters to zero (except CCNT)
	 * [2] - Reset cycle counter (CCNT)
	 */
	asm volatile ("MCR p15, 0, %0, C9, C12, 0\n\t" :: "r" (0x03));

	/* Enable all counters */
	asm volatile ("MCR p15, 0, %0, C9, C12, 1\n\t" :: "r" (0x8000000f));

	/* Clear overflows */
	asm volatile ("MCR p15, 0, %0, C9, C12, 3\n\t" :: "r" (0x8000000f));
	
	#elif defined(__arm64__)

	/**
	 * Same as aarch32 except MCR is no longer supported instruction
	 * so we use MSR to access the system registers directy
	 * 
	 * https://stackoverflow.com/questions/32374599/mcr-and-mrc-does-not-exist-on-aarch64
	 */

	asm volatile ("msr pmuserenr_el0, %0" :: "r" (1));
	asm volatile ("msr pmintenclr_el1, %0" :: "r" (0x8000000f));
	asm volatile ("msr pmcr_el0, %0" :: "r" (0x03));
	asm volatile ("msr pmcntenset_el0, %0" :: "r" (0x8000000f));
	asm volatile ("msr pmovsclr_el0, %0" :: "r" (0x8000000f));

	#endif
}

/* Flush a single block given the virtual address addr */
void flush_one_block(ADDR_PTR addr)
{
	#if defined(__arm__)

	asm volatile(
		"mcr p15, 0, %0, c7, c10, 1"
		:
		: "r" (addr)
		: "memory"
	);

	#elif defined(__arm64__)

	asm volatile (
		"dc civac, %0"
		:
		: "r" (addr)
		: "memory"
	);

	#else

	asm volatile(
		"clflush 0(%0)"
		:
		: "r" (addr)
	);

	#endif
}

/* Measure the time it takes to access a block given a virtual address addr */
CYCLES probe_block(ADDR_PTR addr)
{
	#if defined(__arm__)

	CYCLES cycles_start;
	CYCLES cycles_end;

	asm volatile(
		"dmb ish \n\t"
		"mrc p15, 0, %[start], c9, c13, 0 \n\t"
		"ldr r8, [%[addr]] \n\t"
		"dmb ish \n\t"
		"mrc p15, 0, %[end], c9, c13, 0 \n\t"
		: [start] "=r" (cycles_start), [end] "=r" (cycles_end)
		: [addr] "r" (addr)
		: "r8"
	);

	return cycles_end - cycles_start;

	#elif defined(__arm64__)

	CYCLES cycles_start;
	CYCLES cycles_end;

	asm volatile(
		"dmb ld \n\t"
		"msr pmccntr_el0, %[start]\n\t"
		"ldr x8, [%[addr]] \n\t"
		"dmb ld \n\t"
		"msr pmccntr_el0, %[end]\n\t"
		: [start] "=r" (cycles_start), [end] "=r" (cycles_end)
		: [addr] "r" (addr)
		: "x8"
	);

	return cycles_end - cycles_start;

	#else

	CYCLES cycles;

	asm volatile(
		"mov %1, %%r8       \n\t"	/* move [addr] to register R8 */
		"lfence	            \n\t"	/* ~~~ memory (load) fence ~~~ */
		"rdtsc              \n\t"	/* read timestamp counter in {EDX:EAX} */
		"mov %%eax, %%edi   \n\t"	/* save LS32-bits of time stamp into EDI */
		"mov (%%r8), %%r8   \n\t"	/* load #(R8) into R8 */
		"lfence	            \n\t"	/* ~~~ memory (load) fence ~~~ */
		"rdtsc              \n\t"	/* read timestamp counter again */
		"sub %%edi, %%eax"			/* subtract LS32-bits of timestamp (EAX - EDI) to get the cycles measured */
		: "=a" (cycles)				/* output the accumulator into [cycles] variable */
		: "r" (addr)
		: "r8", "edi");

	return cycles;
	
	#endif
}

void access(void *addr)
{
    #if defined(__arm__)
    asm volatile (
        "ldr r8, [%[addr]]"
        :
        : [addr] "r" (addr)
        : "r8"
    );

	#elif defined(__arm64__)

	asm volatile (
        "ldr x8, [%[addr]]"
        :
        : [addr] "r" (addr)
        : "x8"
    );

    #else
    asm volatile (
        "mov (%0), %%eax"
        :
        : "c" (addr)
        : "eax"
    );
    #endif
}

CYCLES get_cycles()
{
	volatile uint32_t time_lo;
	volatile uint32_t time_hi;

	#if defined(__arm__)

	/* Read cycle count (PMCCNTR is c9, 0, c13, 0) */
	asm volatile (
		"mrc p15, 0, %0, c9, c13, 0"
		: "=r" (time_lo)
	);
	time_hi = 0;

	#elif defined(__arm64__)

	asm volatile (
		"msr pmccntr_el0, %0"
		: "=r" (time_lo)
	);
	time_hi = 0;

	#else

	/**
	 * RDTSC returns the number of cycles since reset
	 * Obtain high resolution cycle/timing information
	 */

	asm volatile(
		"rdtsc"
		: "=a" (time_lo), "=d" (time_hi)
	);

	#endif

	CYCLES time = (uint64_t) time_lo | (((uint64_t) time_hi) << 32);
	return time;
}

void wait_for_time(CYCLES time)
{
	CYCLES start_t = get_cycles();
	while (get_cycles() - start_t < time);
}


#endif