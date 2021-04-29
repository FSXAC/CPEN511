
#include "util.hpp"

/* Padding/dummy functions */
void send_bit_0() { NOP_PADDING }
void send_bit_1() { NOP_PADDING }
void send_bit_2() { NOP_PADDING }
void send_bit_3() { NOP_PADDING }
void send_bit_4() { NOP_PADDING }
void send_bit_5() { NOP_PADDING }
void send_bit_6() { NOP_PADDING }
void send_bit_7() { NOP_PADDING }
void send_bit_8() { NOP_PADDING }

/* Array to hold the covert channel bit addresses */
ADDR_PTR sendbit_arr[BIT_WIDTH];

/* Address to the sender/victim program */
char *mapped;

/**
 * Send a specific bit of a 8-bit character
 * @param: char c - the character
 * @param: int i - the bit index
 */
inline void send_bit(char c, int i)
{
	/* If the bit is "1", then we perform a READ at the covert channel address */
	if (i == 0 || c & (1 << (i - 1)))
		char tmp = mapped[sendbit_arr[i]];
}

int main(void)
{
	printf("=== SENDER PROGRAM ===\n");

	/* Assuming ARM PMU enabled already */

	/**
	 * Setup the covert channel by obtaining the address of the victim
	 * 1. Open the sender program
	 * 2. Map it to memory to get its address
	 */
	#if defined(__arm__)
	int fd = open("sender.arm", O_RDONLY);
	#elif defined(__arm64__)
	int fd = open("sender.arm64", O_RDONLY);
	#else
	int fd = open("sender", O_RDONLY);
	#endif
	mapped = (char *) mmap(NULL, 0x20000, PROT_READ, MAP_PRIVATE, fd, 0);

	/**
	 * For each bit of the transmission, we map it a unique address such that
	 * it has its own cache line
	 */
	int mult = 15;
	sendbit_arr[0] = ADDR_0 + mult * CACHE_LINE_SIZE;
	sendbit_arr[1] = ADDR_1 + mult * CACHE_LINE_SIZE;
	sendbit_arr[2] = ADDR_2 + mult * CACHE_LINE_SIZE;
	sendbit_arr[3] = ADDR_3 + mult * CACHE_LINE_SIZE;
	sendbit_arr[4] = ADDR_4 + mult * CACHE_LINE_SIZE;
	sendbit_arr[5] = ADDR_5 + mult * CACHE_LINE_SIZE;
	sendbit_arr[6] = ADDR_6 + mult * CACHE_LINE_SIZE;
	sendbit_arr[7] = ADDR_7 + mult * CACHE_LINE_SIZE;
	sendbit_arr[8] = ADDR_8 + mult * CACHE_LINE_SIZE;

	/* Ready */
	printf("Victim/sender program now running.\n");
	printf("Enter message to be sent through covert channel:\n");

	bool sending = true;
	while (sending)
	{
		char text_buf[128];

		printf("> ");
		fgets(text_buf, sizeof(text_buf), stdin);
		for (int i = 0; i < 128; i++)
		{
			if (text_buf[i] == '\0')
				break;

			int c = text_buf[i];

			/**
			 * Pulse the bits to the covert channel
			 */
			CYCLES start_t = get_cycles();
			while (get_cycles() - start_t < SLOT_HIT_CUTOFF * TIME_SLOT * SENDER_PULSE)
			{
				send_bit(c, 0);
				send_bit(c, 1);
				send_bit(c, 2);
				send_bit(c, 3);
				send_bit(c, 4);
				send_bit(c, 5);
				send_bit(c, 6);
				send_bit(c, 7);
				send_bit(c, 8);
			}

			wait_for_time(TIME_SLOT * 100);
		}

		#ifdef RUN_ONCE
		sending = false;
		#endif
	}

	return 0;
}
