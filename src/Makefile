# C compiler to use
CC = gcc

# Flags for the compiler
CFLAGS = -O3 -Wall

DBGFLAG = -D DEBUG

# Flags for the linker
LDFLAGS = -lm

all: ballAlg ballQuery

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

.PHONY: clean
clean: 
	@rm -f ballAlg ballQuery
	@rm -f *.o

.PHONY: zip

zip: Makefile *.c *.h 
	@zip src.zip $^