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

/* Number of cycles considered as a threshold to be a hit */
#define TIME_CUTOFF 200

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

/* Flush a single block given the virtual address addr */
void flush_one_block(ADDR_PTR addr)
{
	asm volatile(
		"dc civac, %0"
		:
		: "r" (addr)
		: "memory"
	);
}

// example for add
// int res = 0;
//   __asm ("ADD %[result], %[input_i], %[input_j]"
//     : [result] "=r" (res)
//     : [input_i] "r" (i), [input_j] "r" (j)
//   );
//   return res;

/* Measure the time it takes to access a block given a virtual address addr */
CYCLES probe_block(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile(
		"  mfence             \n" /* DMB ISH LD */
		"  lfence             \n" /* DMB ISH */
		"  rdtsc              \n" /* CCNT */
		"  lfence             \n" /* DMB ISH LD */
		"  movl %%eax, %%esi  \n" /*  */
		"  movl (%1), %%eax   \n"
		"  lfence             \n"
		"  rdtsc              \n"
		"  subl %%esi, %%eax  \n"
		"  clflush 0(%1)      \n"
		: "=a" (cycles)
		: "c" (addr)
		: "%esi", "%edx");

	return cycles;
}

/* Measure the time it takes to access a block given a virtual address addr */
CYCLES probe_block2(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile(
		"mov %1, %%r8       \n"
		"lfence             \n"
		"rdtsc              \n"
		"mov %%eax, %%edi   \n"
		"mov (%%r8), %%r8   \n"
		"lfence	            \n"
		"rdtsc              \n"
		"sub %%edi, %%eax   \n"
		: "=a"(cycles)
		: "r"(addr)
		: "r8", "edi");

	return cycles;
}

CYCLES get_highres_time()
{
	volatile uint32_t time_lo;
	volatile uint32_t time_hi;

	/**
	 * RDTSC returns the number of cycles since reset
	 * Obtain high resolution cycle/timing information
	 * 
	 * ARMv7 has CCNT instruction
	 */
	asm volatile(
		"rdtsc\n"
		: "=a"(time_lo), "=d"(time_hi)
	);

	CYCLES time = (uint64_t)time_lo | (((uint64_t)time_hi) << 32);
	return time;
}

void wait_for_time(CYCLES time)
{
	CYCLES start_t = get_highres_time();
	while(get_highres_time() - start_t < time);
}


#endif