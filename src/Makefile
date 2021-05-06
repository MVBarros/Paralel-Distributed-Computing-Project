# C compiler to use
CC = gcc

# C compiler for MPI
MPICC = mpicc

# Flags for the compiler
CFLAGS = -O3 -Wall

# Flags for the linker
LDFLAGS = -lm

# Flags for debugging
DBGFLAG = -DDEBUG

.PHONY: all clean zip debug

all: ballAlg ballAlg-mpi ballQuery

debug: ballAlg-mpi-debug

ballAlg-mpi: ballAlg-mpi.c gen_points_mpi.o point_operations.o ball_tree.o
	$(MPICC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ballAlg-mpi-debug: ballAlg-mpi.c gen_points_mpi_debug.o point_operations.o ball_tree.o
	$(MPICC) $(CFLAGS) ${DBGFLAG} -fopenmp -o $@ $^ ${LDFLAGS}

ballAlg: ballAlg.c gen_points.o point_operations.o ball_tree.o
	$(CC) $(CFLAGS) -fopenmp -o $@ $^ ${LDFLAGS}

ball_tree.o: ball_tree.c
	$(CC) $(CFLAGS) -c $^

gen_points.o: gen_points.c
	$(CC) $(CFLAGS) -c $^

gen_points_mpi.o: gen_points_mpi.c
	$(CC) $(CFLAGS) -c $^ ${LDFLAGS}

gen_points_mpi_debug.o: gen_points_mpi.c
	$(CC) $(CFLAGS) $(DBGFLAG) -o $@ -c $^

point_operations.o: point_operations.c
	$(CC) $(CFLAGS) -c $^

ballQuery: ballQuery.c
	$(CC) $(CFLAGS) -o $@ $^ ${LDFLAGS}

clean:
	@rm -f ballAlg ballAlg-mpi ballQuery ballAlg-mpi-debug
	@rm -f *.o
	@rm -f *.zip

zip: Makefile *.c *.h
	@zip g29omp.zip $^
