SHELL = /bin/bash # -> POLUS
#SHELL = /bin/sh # -> MY 
SRC = main.cpp functions.cpp tests.cpp
BIN = main
C++FLAGS = -o $(BIN) -std=c++11
C++ = mpic++
#C++POLUS = xlc++ | mpixlc
DATE = $(shell date +%d/%m/%y)
POLUS_MPI_SOURCE = $(shell . mpi_source.sh)

.PHONY: all compile run test clean

all: 
#	-echo $${POLUS_MPI_SOURCE}
	$(MAKE) compile


compile:
	@echo " *  Compiling"
	-$(C++) $(SRC) $(C++FLAGS)
#	-$(C++POLUS) $(SRC) $(C++FLAGS)
	@echo " *  Created on: $(DATE)"
	@echo " *  Author: Aksoy Tevfik Oguzhan, 323"
run:
	mpirun -np $(p) ./main $(s)
	@# P = Processes
	@# S = SIZE

test1:
	mpirun -n 4 ./main 4

clean:
	rm $(BIN)
