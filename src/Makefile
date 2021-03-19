# C compiler to use
CC = gcc

# Flags for the compiler
CFLAGS = -O3 -Wall -std=gnu99

DBGFLAG = -D DEBUG

# Flags for the linker
LDFLAGS = -lm

all: ballAlg ballQuery

ballAlg: ballAlg.c gen_points.o orthogonal.o
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp ${LDFLAGS}

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

orthogonal.o: orthogonal.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

# The phony target "clean" is used to remove all compiled object files.
.PHONY: clean
clean: 
	@rm -f ballAlg ballQuery
	@rm -f *.o