.DEFAULT_GOAL := test

# List of C files in the src/ directory.
SRCS = $(wildcard src/*.c)
# List of object files associated with source files in the src/ directory.
OBJS = $(SRCS:.c=.o)
# Flags to pass to the compiler for building each object file.
FLAGS = -g
# C compiler
CC = gcc
# Include directories
INC = -Iinclude -I../tau

# Build each object file for each file in the src/ directory.
$(OBJS): src/%.o: src/%.c include/%.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

# Build tests object file
tests/tests.o: tests/tests.c
	$(CC) -c $(FLAGS) $(INC) tests/tests.c -o tests/tests.o -w

# Build tests executable
tests/tests.out: $(OBJS) tests/tests.o
	$(CC) $(OBJS) tests/tests.o $(FLAGS) -o tests/tests.out

# Run tests
test: tests/tests.out
	./tests/tests.out

# Clean up all object and executable files
clean:
	rm $(OBJS)
	rm tests/tests.o
	rm tests/tests.out
