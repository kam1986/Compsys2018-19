CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu99 -pthread
EXAMPLES = fibs fauxgrep fauxgrep-mt fhistogram fhistogram-mt 
TESTS = 

.PHONY: all test clean ../src.zip

all: $(TESTS) $(EXAMPLES)

queue.o: queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)

job_queue.o: job_queue.c job_queue.h queue.h 
	$(CC) -c queue.c queue.h $(CFLAGS)
	$(CC) -c job_queue.c $(CFLAGS)

%: %.c job_queue.o queue.o
	$(CC) -o $@ $^ $(CFLAGS)

test: $(TESTS) queue.h queue.o
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

clean:
	rm -rf $(TESTS) $(EXAMPLES) *.o core

../src.zip:
	make clean
	cd .. && zip src.zip -r src
