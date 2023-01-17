# Makefile for mem memory system, CSE374 22WI
# CSE374
# Copyright 2022, Ishan Dane
# 12/12/2022


CC = gcc
CARGS = -Wall -std=c11

all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

# object files
# fill in your individual object targets here
bench.o: bench.c mem.h
	$(CC) $(CARGS) -g -c bench.c mem.h

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CARGS) -g -c getmem.c mem.h mem_impl.h

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CARGS) -g  -c freemem.c mem.h mem_impl.h

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CARGS) -g -c mem_utils.c mem.h mem_impl.h

# You can use these examples for other types of builds
# make sure you know what they do.
noassert: CARGS += -D NDEBUG
noassert: bench

debug: CARGS += -g
debug: bench

test: debug
	./bench 20 50 0

clean:
	rm -f bench *.o *~ 
