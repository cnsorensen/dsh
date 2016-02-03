# Makefile
# Makefile for CSC300 Data Structures PA1 Paint Program

# Author:
# Class: CSC300 Data Structures
# Date: Fall 2015

# Usage: make target1 target2 ...

#------------------------------------------------------------------------------

# GNU C compiler and linker:
LINK = gcc

# Preprocessor and compiler flags (turn on warning, optimization, and debugging):
# CPPFLAGS = <preprocessor flags go here>

CFLAGS = -Wall -O -g
CXXFLAGS = $(CFLAGS)

#---------------------------------------

# Targets
all: dsh

#------------------------------------------------------------------------

dsh: dsh.o
	$(LINK) -o $@ $^ $(LIBS)

#------------------------------------------------------------------------

clean:
	rm -f *.o *~ core

cleanall:
	rm -f *.o *~ core dsh
