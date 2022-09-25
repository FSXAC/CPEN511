CFLAGS = -O1 -pie -std=c99
CCARM64=aarch64-linux-gnu-gcc
PROGRAM = spectre
SOURCE = source.c

all: spectre-v.s spectre-v.arm64

spectre-v.s: $(SOURCE)
	$(CCARM64) $(CFLAGS) -S -o $@ $<

spectre-v.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -static -march=armv8-a -o $@ $<

clean:
	rm -f *.arm64 *.s
