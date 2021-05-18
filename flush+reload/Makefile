
# Must use O0 because otherwise the NOP padding would be optimized away
# It could cause the side channel to not work properly or even SIGBUS when
# trying to read from memory
CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
# CCARM=arm-linux-gnueabihf-gcc
CCARM64=aarch64-linux-gnu-gcc

# x86
all: sender receiver calibration sender.s receiver.s calibration.s

sender: sender.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver: receiver.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration: calibration.cpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

sender.s: sender.cpp
	$(CC) $(CFLAGS) -S $(CEXTRAS) $^

receiver.s: receiver.cpp
	$(CC) $(CFLAGS) -S $(CEXTRAS) $^

calibration.s: calibration.cpp
	$(CC) $(CFLAGS) -S $(CEXTRAS) $^

# # aarch32
# arm: sender.arm receiver.arm calibration.arm

# sender.arm: sender.cpp util.hpp padding.hpp
# 	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

# receiver.arm: receiver.cpp util.hpp padding.hpp
# 	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

# calibration.arm: calibration.cpp util.hpp
# 	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

# aarch64
arm64: sender.arm64 receiver.arm64 calibration.arm64 sender.arm64.s receiver.arm64.s calibration.arm64.s

sender.arm64: sender.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

receiver.arm64: receiver.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

calibration.arm64: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a -static $(CFLAGS) -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^ -o $@

sender.arm64.s: sender.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -S -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^

receiver.arm64.s: receiver.cpp util.hpp padding.hpp
	$(CCARM64) -march=armv8-a -S -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^

calibration.arm64.s: calibration.cpp util.hpp
	$(CCARM64) -march=armv8-a -S -D__arm64__ -DRUN_ONCE $(CEXTRAS) $^


.PHONY: clean

clean:
	rm -f $(HELPERS) sender receiver calibration
	rm -f *.arm
	rm -f *.arm64
	rm -f receiver_out.txt
	rm -f out.txt
	rm -f *.s
	rm -f *.gch