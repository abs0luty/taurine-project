sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g
cc = gcc

%.o: %.c include/%.h
	$(cc) -c $(flags) $< -o $@

tests.o: tests.c
	$(cc) -c $(flags) tests.c -o tests.o -w

tests.out: $(objects) tests.o
	$(cc) $(objects) tests.o $(flags) -o tests.out

test: tests.out
	./tests.out

clean:
	rm $(objects)
	rm tests.o
	rm tests

.PHONY: test
