cc = gcc
CFLAGS = -Wall -Wextra -Werror -g

all: main

leak: main
	valgrind --leak-check=full -s ./main

main: main.c libgoopy.a
	$(cc) $(CFLAGS) main.c -Llib/ -lgoopy -lm -o $@

libgoopy.a: goopy.o
	mkdir -p lib
	ar rcs lib/libgoopy.a build/$^

goopy.o: goopy/goopy.c
	$(cc) -c $^ -o build/$@

clean:
	rm -rf lib/* build/*
	rm -f  main
