# Flush+Reload Implementation for x86 and ARM

## Usage (x86)

In the `flush+reload` directory, use the command `make` to build three executables:

- calibration
- sender
- receiver

The *calibration* program performs a test on cache hit/miss timings and generate a histogram (printed on screen or to a csv file).

The *sender* and *receiver* program must exist in the same directory and must run at the same time. This is because the receiver program reads the sender file to know where it is in virtual memory -- (attacker knows victim's address). A message sent in the sender program can then be transmitted to the receiver program via the cache timing side channel.

**Receiver program argument**: when launching the receiver program, we can add an argument: `./receiver 150` to specify a cache hit/miss cycle threshold. By default this is set to 200 cycles.

**RUN_ONCE preprocessor define**: by default, both sender and receiver application will run indefinitely as a demo, but if we want to perform some test/automation, then we want the sender/receiver pair to run once, send a message, and exit (and check the output afterwards to verify side channel integrity). To do this, add the #define during make: `make CEXTRAs=-DRUN_ONCE`.

### Auto Tests

To make the above process simpler, there are two scripts:

- autotest.py: This python script sweeps various hit/miss threshold with a preloaded message and outputs accuracy of the message sent over the side channel
- autotest.sh: This shell script simply runs a the side channel one time with a preloaded message -- useful for running in the simulator where there is no python.

## Usage (ARM)

Because I don't have an ARM computer, we need to crosscompile it into ARM binaries, then copy it into the ARM simulator and run it from there.

### Crosscompiling to ARM

This procedure is taken mostly form <https://www.96boards.org/documentation/guides/crosscompile/commandline.html>

- Download the toolchain required to compile aarch32 or aarch64

  ```shell
  # aarch32
  sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

  # aarch64
  sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
  ```
- Compile using the crosscompiler
  ```shell
  # aarch32
  arm-linux-gnueabihf-gcc <source_file.c> -o <out_name>.arm32

  # aarch64
  aarch64-linux-gnu-gcc <source _ile.c> -o <out_name>.arm64
  ```
- Check the output using the `file <binary>` command

Instead of calling the compiler manually, we can instead override the compiler variable in the Makefile: `make CC=aarch64-linux-gnu-g++` for example.