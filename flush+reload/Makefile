CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++
# CCARM=aarch64-linux-gnu-gcc
CCARM=arm-linux-gnueabihf-gcc

all: sender receiver calibration

sender: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

all_arm: sender.arm receiver.arm calibration.arm

sender.arm: sender.cpp util.hpp padding.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

receiver.arm: receiver.cpp util.hpp padding.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

calibration.arm: calibration.cpp util.hpp
	$(CCARM) $(CFLAGS) -D__arm__ $(CEXTRAS) $^ -o $@

.PHONY: clean

clean:
	rm $(HELPERS) sender receiver calibration

clean_arm:
	rm $(HELPERS) *.arm64
