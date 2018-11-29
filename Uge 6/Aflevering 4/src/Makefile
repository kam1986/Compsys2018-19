CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Winline -Og

X86PRIME = x86prime

PROGRAMS = quicksort heapsort yoursort

all:
	make $(PROGRAMS)

../src.zip: Makefile yoursort.c heapsort.c quicksort.c x86prime_lib.h data/ tests/ README run-performance.sh run-test.sh
	make clean
	cd .. && zip src.zip src/Makefile src/yoursort.c src/heapsort.c src/quicksort.c src/x86prime_lib.h src/data/* src/tests/* src/README src/run-performance.sh src/run-test.sh

quicksort: quicksort.c
	$(CC) $(CFLAGS) -S $<
	$(X86PRIME) -f $@.s -txl -list > $@.s_prime

heapsort: heapsort.c
	$(CC) $(CFLAGS) -S $<
	$(X86PRIME) -f $@.s -txl -list > $@.s_prime

yoursort: yoursort.c
	$(CC) $(CFLAGS) -S $<
	$(X86PRIME) -f $@.s -txl -list > $@.s_prime

clean:
	rm -f $(foreach prg,$(PROGRAMS),$(prg).s)
	rm -f $(foreach prg,$(PROGRAMS),$(prg).s_prime)
	rm -rf runs
	rm -rf test_runs
