CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

# x86
all: sender.x86 receiver.x86 calibration.x86

sender.x86: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver.x86: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration.x86: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

# aarch64
arm64: sender.arm64 receiver.arm64 calibration.arm64

sender.arm64: sender.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

receiver.arm64: receiver.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

calibration.arm64: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

.PHONY: clean

clean:
	rm -f $(HELPERS)
	rm -f *.x86
	rm -f *.arm
	rm -f *.arm64
	rm -f calibration_stats.csv
