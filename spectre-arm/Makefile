CFLAGS = -O1 -pie -std=c99
CCARM64=aarch64-linux-gnu-gcc
PROGRAM = spectre
SOURCE = source.c

all: spectre.arm64 spectre-debug.arm64 spectre-static.arm64 spectre-mix.arm64 spectre.x86 spectre-static.x86

spectre.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -march=armv8-a -o $@ $<

spectre-debug.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -static -DDEBUG -march=armv8-a -o $@ $<

spectre-static.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -static -march=armv8-a -o $@ $<

spectre-mix.arm64: $(SOURCE)
	$(CCARM64) $(CFLAGS) -DMIX -march=armv8-a -o $@ $<

spectre.x86: $(SOURCE)
	gcc -O0 -pie -std=c99 -DMIX -DX86 -o $@ $<

spectre-static.x86: $(SOURCE)
	gcc -O0 -pie -std=c99 -DMIX -DX86 -static -o $@ $<

clean:
	rm -f *.arm64 *.x86
