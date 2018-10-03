CC = gcc
CFLAGS = -Wall -O0 -std=c11 -ggdb

all: sim

../src.zip: arithmetic.c arithmetic.h compute.c compute.h ip_reg.c ip_reg.h main.c Makefile memory.c memory.h registers.c registers.h support.c support.h trace_read.h trace_read.c wires.c wires.h
	cd .. && zip src.zip src/arithmetic.c src/arithmetic.h src/compute.c src/compute.h src/ip_reg.c src/ip_reg.h src/main.c src/Makefile src/memory.c src/memory.h src/registers.c src/registers.h src/support.c src/support.h src/trace_read.h src/trace_read.c src/wires.c src/wires.h


sim: *.c
	$(CC) $(CFLAGS) *.c -o sim

clean:
	rm -f *.o sim
