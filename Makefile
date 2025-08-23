BUILD=build
CC=gcc
CFLAGS=-Wall -Wextra -g

all: main $(BUILD)/goopy.o

main: main.c $(BUILD)/goopy.o
	$(CC) $(CFLAGS) main.c $(BUILD)/goopy.o -lm -o $@

$(BUILD)/goopy.o: goopy.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -pv $(BUILD)

leak: main
	valgrind --leak-check=full -s ./main

clean:
	rm -rf build/*
	rm -f  main
