# C compiler to use
CC = gcc

# Flags for the compiler
CFLAGS = -O3 -Wall -std=gnu99

# Flags for the linker
LDFLAGS = -lm

.PHONY: all clean zip

all: ballAlg ballAlg-omp ballQuery

ballAlg-omp: ballAlg-omp.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp ${LDFLAGS}

ballAlg: ballAlg.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp ${LDFLAGS}

ball_tree.o: ball_tree.c
	$(CC) $(CFLAGS) -c $^

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

point_operations.o: point_operations.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

clean: 
	@rm -f ballAlg ballQuery ballAlg-omp
	@rm -f *.o

zip: Makefile *.c *.h 
	@zip g29omp.zip $^
