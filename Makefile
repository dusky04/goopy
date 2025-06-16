cc = gcc
CFLAGS = -Wall -Wextra -Werror -g

main: main.c goopy.c goopy.h
	$(cc) $(CFLAGS) $^ -o $@ -lm


clean:
	rm -f main
