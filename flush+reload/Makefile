CFLAGS=-std=c++11 -I /usr/local -O0
CEXTRAS=
CC=g++

all: sender receiver calibration

sender: sender.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

receiver: receiver.cpp util.hpp padding.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

calibration: calibration.cpp util.hpp
	$(CC) $(CFLAGS) $(CEXTRAS) $^ -o $@

.PHONY: clean

clean:
	rm *.o $(HELPERS) sender receiver calibration
