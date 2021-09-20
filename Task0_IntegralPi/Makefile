CC = gcc
all: main.c
	$(CC) main.c -o main -Wall -Werror -pthread

test:
	$(CC) main.c -o main -Werror -Wall -pthread -fsanitize=address
	sh test.sh
	rm main
	$(CC) main.c -o main -Werror -Wall -pthread

clean:
	rm main
