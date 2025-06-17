cc = gcc
# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -Wall -Wextra -g

all: main

leak: main
	valgrind --leak-check=full -s ./main

main: main.c goopy.c goopy.h
	$(cc) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -f main
