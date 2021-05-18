# CFLAGS=-std=c++11 -I /usr/local -O2
CFLAGS=-std=c++11
CEXTRAS=
CC=gcc
CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

# x86
all: sender receiver calibration

sender:
	$(CC) $(CFLAGS) sender.cpp -o sender.x86

receiver: receiver.cpp
	$(CC) $(CFLAGS) receiver.cpp -o receiver.x86

calibration: calibration.cpp
	$(CC) $(CFLAGS) calibration.cpp -o calibration.x86

# aarch32
arm: sender.arm receiver.arm calibration.arm

sender.arm: sender.cpp util.hpp padding.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

receiver.arm: receiver.cpp util.hpp padding.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

calibration.arm: calibration.cpp util.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

# aarch64
arm64: sender.arm64 receiver.arm64 calibration.arm64

sender.arm64: sender.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

receiver.arm64: receiver.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

calibration.arm64: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ $(CEXTRAS) $^ -o $@

calibration.s.arm64: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a -S -O0 -D__arm64__ $(CEXTRAS) $^


.PHONY: clean

clean:
	rm -f *.x86
	rm -f *.arm
	rm -f *.arm64
