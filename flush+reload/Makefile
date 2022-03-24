# Must use O0 because otherwise the NOP padding would be optimized away
# It could cause the side channel to not work properly or even SIGBUS when
# trying to read from memory
CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
CCARM64=aarch64-linux-gnu-gcc

# x86
all: sender receiver calibration
sender:
	$(CC) $(CFLAGS) sender.cpp -o sender.x86

receiver: receiver.cpp
	$(CC) $(CFLAGS) receiver.cpp -o receiver.x86

calibration: calibration.cpp
	$(CC) $(CFLAGS) calibration.cpp -o calibration.x86

# aarch64
arm64: sender.arm64 receiver.arm64 calibration.arm64

sender.arm64: sender.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

receiver.arm64: receiver.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

calibration.arm64: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

.PHONY: clean

clean:
	rm -f *.x86
	rm -f *.arm
	rm -f *.arm64
	rm -f receiver_out.txt
	rm -f out.txt
	rm -f *.s
	rm -f *.gch
