CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude

SRC = src/main.c
INSPECT_SRC = src/inspect.c

INSPECT_OUT = bin/inspect

all: $(INSPECT_OUT)

# Build inspect
$(INSPECT_OUT): $(SRC) $(INSPECT_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(INSPECT_SRC) -o $(INSPECT_OUT)

# Run inspect on a GGUF file
inspect: $(INSPECT_OUT)
	$(INSPECT_OUT) $(MODEL)

# Run inspect under valgrind
valgrind: $(INSPECT_OUT)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(INSPECT_OUT) $(MODEL)

clean:
	rm -f bin/*

.PHONY: all inspect valgrind clean

