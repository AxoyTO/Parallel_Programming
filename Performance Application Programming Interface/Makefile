CC = gcc
CFLAGS = -Wall -Werror -O0 -lpapi -std=c99
BIN = -o main

.PHONY: all bin test test32 test64 longrun clean

all: main.c
	$(CC) main.c $(BIN) $(CFLAGS)

bin: binary.c
	$(CC) binary.c -o bin -std=c99

run: bin all
	sh run.sh
	rm main

longrun: bin all
	sh longrun.sh
	rm main

test:
	$(CC) binary.c -o bin -fsanitize=address -std=c99
	$(CC) main.c $(BIN) $(CFLAGS) -fsanitize=address
	sh test.sh
	rm main bin a b c

test32: bin all
	sh test32.sh
	rm main bin a b c

test64: bin all
	sh test64.sh
	rm main bin a b c

clean:
	rm main bin a b c


