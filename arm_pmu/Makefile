CC=gcc
CCARM64=aarch64-linux-gnu-gcc

all: perf_event_open.arm64

perf_event_open.arm64: perf_event_open.c
	$(CCARM64) -std=gnu99 perf_event_open.c -o perf_event_open.arm64