# C compiler to use
CC = gcc

# C compiler for MPI
MPICC = mpicc

# Flags for the compiler
CFLAGS = -O3 -Wall

# Flags for the linker
LDFLAGS = -lm

.PHONY: all clean zip

all: ballAlg ballAlg-mpi ballQuery

ballAlg-mpi: ballAlg-mpi.c gen_points_mpi.o point_operations.o ball_tree.o sort_mpi.o
	$(MPICC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ballAlg: ballAlg.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ball_tree.o: ball_tree.c
	$(CC) $(CFLAGS) -c $^

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

gen_points_mpi.o: gen_points_mpi.c
	$(MPICC) $(CFLAGS) -c $^ ${LDFLAGS}

sort_mpi.o: sort_mpi.c
	$(MPICC) $(CFLAGS) -c $^ ${LDFLAGS}

point_operations.o: point_operations.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

clean:
	@rm -f ballAlg ballAlg-mpi ballQuery
	@rm -f *.o
	@rm -f *.zip

zip: Makefile *.c *.h
	@zip g29mpi.zip $^
