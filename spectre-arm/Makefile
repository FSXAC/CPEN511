CFLAGS = -O0 -pie -Wall -Wextra -std=c99
LIBFLUSH_CFLAGS = -I. -L.
CCARM64=aarch64-linux-gnu-gcc
PROGRAM = spectre
SOURCE = source.c

all: spectre.arm64 spectre.x86

spectre.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -DTIMING_REGISTER -march=armv8-a -o $@ $<

spectre.x86: $(SOURCE)
	gcc $(CFLAGS) -DTIMING_REGISTER -DX86 -o $@ $<

clean:
	rm -f *.arm64 *.x86
