# Makefile for Lab 4:
#
# Written by Jerod Weinman, 14 January 2015
# Extended by Henry M. Walker, 15 January 2015
# Preprocessor flags added by Jerod Weinman, 19 January 2015
# Modified by Rexford Essilfie for radixsort.c assignment, 20 February  2020

# Type "make foo".
# We rely on the implicit  rules foo.c -> foo.o for compiling and
# foo.o --> foo for linking

# Use the clang compiler
CC = gcc

# Set appropriate compiling flags
CFLAGS= -ansi -g -Wall -std=gnu99


#----------------------------------------------------------------------------
# cleanup rules: To invoke this command, type "make clean".
# Use this target to clean up your directory, deleting (without warning)
#   object files, old emacs source versions, and core dumps.

all: radixsortTest

radixsortTest: radixsortTest.o
	$(CC) -o radixsortTest radixsortTest.o

radixsort: radixsort.o
	$(CC) -o radixsort radixsort.o

radixsort.o: radixsort.c
	$(CC) -c radixsort.c 

radixsortTest.o: radixsortTest.c
	$(CC) -c radixsortTest.c

clean:
	rm -f *.o *~ core*

clear:
	rm -f *.o *~ core*
	rm -f radixsortTest
