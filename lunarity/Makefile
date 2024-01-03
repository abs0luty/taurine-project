# List of C files in the src/ directory.
SRCS = $(wildcard src/*.c)
# List of object files associated with source files in the src/ directory.
OBJS = $(SRCS:.c=.o)
# Flags to pass to the compiler for building each object file.
FLAGS = -g
# C compiler
CC = gcc
# Include directories
INC = -Iinclude -I../tau -I../arty/include

# Build each object file for each file in the src/ directory.
$(OBJS): src/%.o: src/%.c include/%.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

tests/lexer_tests.o: tests/lexer_tests.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

# Build the executable
tests/lexer_tests.out: $(OBJS) tests/lexer_tests.o
	$(CC) -o $@ $(OBJS) tests/lexer_tests.o

# Test
test: tests/lexer_tests.out
	./tests/lexer_tests.out

# Clean up all object and executable files
clean:
	rm $(OBJS)
	rm tests/lexer_tests.o
	rm tests/lexer_tests.out
