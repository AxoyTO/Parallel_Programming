SHELL = /bin/bash # -> POLUS
#SHELL = /bin/sh # -> MY 
SRC = main.cpp functions.cpp
BIN = main
C++FLAGS = -o $(BIN) -std=c++17 -fopenmp
C++ =  g++
#C++POLUS = xlc++ | mpixlc | xlc++_r
DATE = $(shell date +%d/%m/%y)
POLUS_MPI_SOURCE = $(shell . mpi_source.sh)
COMPUTER ?= Desktop


ifeq ($(COMPUTER), Polus)
C++ = xlc++_r
endif


.PHONY: all compile run test clean

all: clean compile
#	-echo $${POLUS_MPI_SOURCE}
#	$(MAKE) test


compile:
	@echo " *  Compiling"
	-$(C++) $(SRC) $(C++FLAGS)
#	-$(C++POLUS) $(SRC) $(C++FLAGS)
	@echo " *  Created on: $(DATE)"
	@echo " *  Author: Aksoy Tevfik Oguzhan, 323"

run:
	./main $(N) $(maxiter) $(threads)
	@# N = Size of cube's one side(Nx = Ny = Nz)
	@# maxiter = Maximum iterations
	@# threads = Number of OMP Threads

test:
	./main 3 4 6

clean:
	rm -rf $(BIN)
