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

# Clean up all object and executable files
clean:
	rm $(OBJS)
	rm tests/tests.o
	rm tests/tests.out
