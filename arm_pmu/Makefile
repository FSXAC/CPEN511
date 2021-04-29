CC=gcc
CCARM64=aarch64-linux-gnu-gcc

all: perf_event_open.arm64 pe2.arm64

perf_event_open.arm64: perf_event_open.c
	$(CCARM64) -static -std=gnu99 perf_event_open.c -o perf_event_open.arm64

pe2.arm64: pe2.c
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@