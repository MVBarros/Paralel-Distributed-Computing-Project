# C compiler to use
CC = gcc

# Flags for the compiler
CFLAGS = -O3 -Wall -std=gnu99

DBGFLAG = -D DEBUG

# Flags for the linker
LDFLAGS = -lm

all: clean ballAlg ballQuery

ballAlg: ballAlg.c gen_points.o point_operations.o
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp ${LDFLAGS}

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

point_operations.o: point_operations.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

# The phony target "clean" is used to remove all compiled object files.
.PHONY: clean
clean: 
	@rm -f ballAlg ballQuery
	@rm -f *.o