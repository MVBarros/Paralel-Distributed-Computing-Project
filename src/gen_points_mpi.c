#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "gen_points_mpi.h"
#include "macros.h"

#define RANGE 10

extern int n_procs;
extern int rank;

extern void print_point(double *, int);

double **create_array_pts(int n_dims, long np)
{
    double *_p_arr;
    double **p_arr;

    _p_arr = (double *) malloc(n_dims * np * sizeof(double));
    p_arr = (double **) malloc(np * sizeof(double *));
    if((_p_arr == NULL) || (p_arr == NULL)){
        printf("Error allocating array of points, exiting.\n");
        exit(4);
    }

    for(long i = 0; i < np; i++)
        p_arr[i] = &_p_arr[i * n_dims];

    return p_arr;
}

void free_array_pts(double ** p_arr)
{
    free(p_arr[0]);
    free(p_arr);
}

void realoc_array_pts(double ** p_arr, int n_dims, long curr_size, long new_size)
{
    if (curr_size < new_size) {
        free_array_pts(p_arr);
        *p_arr = *create_array_pts(n_dims, new_size);
    }
}

double **get_points(int argc, char *argv[], int *n_dims, long *np)
{
    double **pt_arr;
    unsigned seed;
    long i;
    int j;

    if(argc != 4){
        printf("Usage: %s <n_dims> <n_points> <seed>\n", argv[0]);
        exit(1);
    }

    *n_dims = atoi(argv[1]);
    if(*n_dims < 2){
        printf("Illegal number of dimensions (%d), must be above 1.\n", *n_dims);
        exit(2);
    }

    *np = atol(argv[2]);
    if(*np < 1){
        printf("Illegal number of points (%ld), must be above 0.\n", *np);
        exit(3);
    }

    seed = atoi(argv[3]);
    srandom(seed);

    long np_local = BLOCK_SIZE(rank, n_procs, *np);
    long low = BLOCK_LOW(rank, n_procs, *np);

    pt_arr = (double **) create_array_pts(*n_dims, (long) (ceil((double) (*np) / (double) (n_procs)))); //Overfit just in case

    for(i = 0; i < low; i++) {
        for(j = 0; j < *n_dims; j++) {
            random(); //advance random generator
        }
    }

    for(i = 0; i < np_local; i++)
        for(j = 0; j < *n_dims; j++)
            pt_arr[i][j] = RANGE * ((double) random()) / RAND_MAX;

    return pt_arr;
}
