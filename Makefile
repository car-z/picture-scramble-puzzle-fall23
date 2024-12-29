GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

puzzle: puzzle.o puzzle_funcs.o ppm_io.o
	$(GCC) -o puzzle puzzle.o puzzle_funcs.o ppm_io.o

puzzle.o: puzzle.c puzzle.h ppm_io.h
	$(GCC) -c puzzle.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c puzzle.h ppm_io.h
	$(GCC) -c puzzle_funcs.c $(CFLAGS)

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c $(CFLAGS)

puzzle_tests.o: puzzle_tests.c puzzle.h ppm_io.h
	$(GCC) -c puzzle_tests.c $(CFLAGS)

test: puzzle_funcs.o puzzle_tests.o ppm_io.o
	$(GCC) -o test puzzle_funcs.o puzzle_tests.o ppm_io.o

clean:
	rm -f *.o puzzle test *~


