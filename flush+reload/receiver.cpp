#include "util.hpp"

int main(void)
{
	/**
	 * Open the same file/victim/sender program
	 * and map it in memory (since both victim and receiver knows the address)
	 */
	int fd = open("sender", O_RDONLY);
	char *mapped = (char *) mmap(NULL, 0x20000, PROT_READ, MAP_PRIVATE, fd, 0);

	// int num_ptrs = 0;
	ADDR_PTR probe_ptrs[BIT_WIDTH];
	int mult = 15;

	/**
	 * ADDR_X + mult * CACHE_LINE_SIZE maps to the same cacheblocks of the 
	 * bits sent from the sender of the covert channel
	 * 
	 */
	probe_ptrs[0] = (ADDR_PTR) (mapped + ADDR_0 + mult * CACHE_LINE_SIZE);
	probe_ptrs[1] = (ADDR_PTR) (mapped + ADDR_1 + mult * CACHE_LINE_SIZE);
	probe_ptrs[2] = (ADDR_PTR) (mapped + ADDR_2 + mult * CACHE_LINE_SIZE);
	probe_ptrs[3] = (ADDR_PTR) (mapped + ADDR_3 + mult * CACHE_LINE_SIZE);
	probe_ptrs[4] = (ADDR_PTR) (mapped + ADDR_4 + mult * CACHE_LINE_SIZE);
	probe_ptrs[5] = (ADDR_PTR) (mapped + ADDR_5 + mult * CACHE_LINE_SIZE);
	probe_ptrs[6] = (ADDR_PTR) (mapped + ADDR_6 + mult * CACHE_LINE_SIZE);
	probe_ptrs[7] = (ADDR_PTR) (mapped + ADDR_7 + mult * CACHE_LINE_SIZE);
	probe_ptrs[8] = (ADDR_PTR) (mapped + ADDR_8 + mult * CACHE_LINE_SIZE);

	printf("Receiver now listening.\n");

	/**
	 * Receiver collection bin
	 */
	int hits[BIT_WIDTH];

	bool listening = true;
	while (listening)
	{
		/**
		 * The spy clears the hit statistics
		 */
		for (int i = 0; i < BIT_WIDTH; i++)
			hits[i] = 0;

		
		/**
		 * Spy waits
		 */
		for (int j = 0; j < SLOT_WINDOW_COUNT; j++)
		{

			/**
			 * For each slot (correspond to each bit being transmitted),
			 * perform a cache line flush for that address
			 */
			for (int i = 0; i < BIT_WIDTH; i++)
				flush_one_block(probe_ptrs[i]);

			/**
			 * Wait for victim to access the gadget and
			 * perform a load on the same address
			 */
			wait_for_time(TIME_SLOT);


			/**
			 * The spy performs re-load on the slots who corresponds to the
			 * covert channel bits and measure the time -- if the access time
			 * is short, then we know we have a "hit"
			 * 
			 * Then increment the hits array to track stats -- effectively
			 * exfiltrating data from the covert channel
			 */
			for (int i = 0; i < BIT_WIDTH; i++)
			{
				CYCLES time = probe_block(probe_ptrs[i]);
				flush_one_block(probe_ptrs[i]);
				if (time < TIME_CUTOFF)
					hits[i]++;
			}
		}

		/**
		 * Check 0th bit as it means we got message!
		 */
		if (hits[0] >= SLOT_HIT_CUTOFF)
		{
			/**
			 * Reconstruct the character from the bit stream
			 */
			int bitstr = 0;
			for (int i = 1; i < BIT_WIDTH; i++)
				if (hits[i] >= SLOT_HIT_CUTOFF)
					bitstr |= 1 << (i - 1);

			/**
			 * Print the character
			 */
			printf("%c", (char) bitstr);
			fflush(stdout);
		}
	}

	return 0;
}
