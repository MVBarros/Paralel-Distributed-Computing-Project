# C compiler to use
CC = gcc

#C compiler for omp
CC_OMPP = kinst-ompp gcc

# Flags for the compiler
CFLAGS = -O3 -Wall

# Flags for the linker
LDFLAGS = -lm

.PHONY: all ompp clean zip

all: ballAlg ballAlg-omp ballQuery

ompp: ballAlg-ompp ballQuery

ballAlg-ompp: ballAlg-omp.c gen_points.o point_operations.o ball_tree.o
	$(CC_OMPP) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ballAlg-omp: ballAlg-omp2.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ballAlg: ballAlg.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

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
	@rm -f *.zip

zip: Makefile *.c *.h
	@zip g29omp.zip $^
