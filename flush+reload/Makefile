CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

# x86
all: sender receiver calibration

sender: sender.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver: receiver.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration: calibration.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

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
	rm -f $(HELPERS) sender receiver calibration
	rm -f *.arm
	rm -f *.arm64
