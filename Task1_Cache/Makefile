CC = gcc
CFLAGS = -Wall -Werror -O0
BIN = -o main

.PHONY: all bin test test32 test64 clean

all: main.c
	$(CC) main.c $(BIN) $(CFLAGS)

bin: binary.c
	$(CC) binary.c -o bin

run: bin all
	sh run.sh
	rm main

longrun: bin all
	sh longrun.sh
	rm main

test:
	$(CC) binary.c -o bin -fsanitize=address
	$(CC) main.c $(BIN) $(CFLAGS) -fsanitize=address
	sh test.sh
	rm main

test32:
	$(CC) binary.c -o bin
	$(CC) main.c $(BIN) $(CFLAGS)
	sh test32.sh
	rm main

test64:
	$(CC) binary.c -o bin
	$(CC) main.c $(BIN) $(CFLAGS)
	sh test64.sh
	rm main

clean:
	rm main a b c


