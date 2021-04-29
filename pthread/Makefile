CFLAGS=-std=c++11 -I /usr/local -O2
CEXTRAS=-pthread
CC=g++
# CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

pthread.x86: pt.h pt.c
	$(CC) -march=native $(CFLAGS) $(CEXTRAS) $^ -o $@

arm64: pthread.arm64 pthread-static.arm64

pthread.arm64: pt.h pt.c
	$(CCARM64) -march=armv8-a $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

pthread-static.arm64: pt.h pt.c
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@
