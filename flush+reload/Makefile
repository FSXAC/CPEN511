CFLAGS=-std=c++11 -I /usr/local -O0
CC=g++

all: sender receiver

sender: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $^ -o $@

receiver: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $^ -o $@

calibration: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $^ -o $@ -O2

.PHONY: clean

clean:
	rm *.o $(HELPERS) sender receiver
