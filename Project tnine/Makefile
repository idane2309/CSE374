# CSE 374 HW 5 Makefile for t9 program.
# Copyright 2022 Ishan Dane
# 11/23/2022

# This is a "phony" target
# that tells make what other targets to build.
all: t9

# The executable
t9: trie.o tnine.o
	gcc -Wall -std=c11 -g -o t9 trie.o tnine.o

# Individual source files
trie.o: trie.c trienode.h
	gcc -Wall -std=c11 -g -c trie.c trienode.h

tnine.o: tnine.c trienode.h
	gcc -Wall -std=c11 -g -c tnine.c trienode.h

# A "phony" target to remove built files and backups
clean:
	rm -f *.o t9 *~
