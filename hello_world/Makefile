CFLAGS=-std=c++11 -I /usr/local -O2
CEXTRAS=
CC=g++
CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

hello.x86: hello.cpp
	$(CC) -march=native $(CFLAGS) $(CEXTRAS) $^ -o $@

arm64: hello.arm64 hello-static.arm64

hello.arm64: hello.cpp
	$(CCARM64) -march=armv8-a $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

hello-static.arm64: hello.cpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@
