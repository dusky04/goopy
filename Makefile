cc = gcc
CFLAGS = -Wall -Wextra -g

main: main.c goopy.c goopy.h
	$(cc) $(CFLAGS) $^ -o main


clean:
	rm -f main
