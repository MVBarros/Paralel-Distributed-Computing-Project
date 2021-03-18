# C compiler to use
CC = gcc

# Flags for the compiler
CFLAGS = -O3 -Wall -std=gnu99

# Flags for the linker
LDFLAGS = -lm

all: ballAlg ballQuery

ballAlg: ballAlg.c gen_points.o
	$(CC) $(CFLAGS) -o $@ $^

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

# The phony target "clean" is used to remove all compiled object files.
.PHONY: clean
clean: 
	@rm -f ballAlg ballQuery
	@rm -f *.o