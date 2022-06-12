CC = gcc
CFLAGS = -Wall -Werror -O0 -fopenmp -std=c11
BIN = -o SortOMP
DATE = $(shell date +%d/%m/%y)


.PHONY: all run test clean submitpolus cleanpolus

all: SortOMP.c
	@echo " *  Compiling with flags $(CFLAGS)"
	@$(CC) SortOMP.c $(BIN) $(CFLAGS)
	@#@echo " *  Created on: $$(date +%d/%m/%y)"	#@ hides the command output, like make -s
	@echo " *  Created on: $(DATE)"
	@echo " *  Author: Aksoy Tevfik Oguzhan, 323"
	

run: all
	@echo " *  Running shell script 'run.sh'"
	@sh run.sh


test:
	#fsanitize=address doesn't exist in POLUS
	@echo " *  Compiling with flags $(CFLAGS) -fsanitize=address"
	@$(CC) SortOMP.c $(BIN) $(CFLAGS) -fsanitize=address
	@echo " *  Test successful!"
	@echo " *  Deleting the binary file."
	@rm SortOMP

clean:
	rm SortOMP

submitpolus:
	mpisubmit.pl --stdout result.out --stderr result.err make run

cleanpolus:
	rm result.err result.out