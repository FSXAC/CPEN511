CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
CCARM=aarch64-linux-gnu-g++

all: sender receiver calibration

sender: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

all_arm: sender.arm64 receiver.arm64 calibration.arm64

sender.arm64: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver.arm64: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration.arm64: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

.PHONY: clean

clean:
	rm $(HELPERS) sender receiver calibration

clean_arm:
	rm $(HELPERS) *.arm64
