CC = gcc
MPICC = mpixlc
CCFLAGS = -Wall -Werror -std=c11
SRC = binary.c
BIN = binary
MPISRC = main.c
MPIBIN = main

.PHONY: all bin test clear

all: $(MPISRC)
	@echo " *  Compiling $(MPISRC) with $(MPICC)"
	@$(MPICC) $(MPISRC) -o $(MPIBIN)

bin: binary.c
	@echo " *  Compiling $(SRC) with $(CC)"
	@$(CC) $(SRC) -o $(BIN)

test:
	@echo " *  Compiling $(CC) with flags $(CFLAGS)"
	@$(CC) $(SRC) -o $(BIN) $(CCFLAGS)
	@echo " *  Compiling $(MPICC)"
	@$(MPICC) $(MPISRC) -o $(MPIBIN)
	@echo " *  Running test script"
	@sh test.sh
	@echo " *  Test successful!"
	@echo " *  Deleting the binary files."
	@rm main binary

clear:
	@echo " *  Deleting the binary files."
	@rm main binary
