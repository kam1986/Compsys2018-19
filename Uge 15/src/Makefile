CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu11
LD_FLAGS = -lpthread


test: Login Logout Lookup Send Readline Sourcedump 

all : peer name_server test

rebuild : clean all

csapp.o : csapp.c csapp.h
	$(CC) $(CFLAGS) -c $< -o $@

help : help.c help.h csapp.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

peer : peer.c peer.h csapp.o help.o 
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

name_server : name_server.c name_server.h csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

clean :
	rm -rf *.o peer name_server
	rm -rf ./Tests/*.test
	rm -rf temp
	rm -rf test

../src.zip :
	make clean
	cd .. && zip src.zip -r src


Login: ./Tests/Login.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)

Logout: ./Tests/Logout.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)

Lookup: ./Tests/Lookup.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)

Send: ./Tests/Send.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)


Readline: ./Tests/Readline.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)

Sourcedump: ./Tests/Sourcedump.c csapp.o help.o
	$(CC) $(CFLAGS) $< *.o -o ./Tests/$@.test $(LD_FLAGS)
