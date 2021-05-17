#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include "point_operations.h"
#include "gen_points.h"
#include "macros.h"
#include "point_utils_mpi.h"

extern double **ortho_array;

extern long n_points_local;
extern long n_dims;
extern long n_points_global;

extern long *processes_n_points;

extern int rank;
extern int n_procs;

extern MPI_Comm communicator;

/*
Places in recv_counts how many data elements each process will send in the naive_get_center implementation.
Places in displays the displacement of the data received by each process in the naive_get_center implementation.
*/
void naive_compute_receive_info(int *recv_counts, int *displays) {
    int displacement = 0;
    for(int i = 0; i < n_procs; i++) {
        recv_counts[i] = processes_n_points[i] * n_dims; /* receiving processes_n_points[i] * n_dims doubles from process i */
        displays[i] = displacement * n_dims; /* data received from process i will start in index display */
        displacement += processes_n_points[i];
    }
}

/*
Copies the median projection of sorted_projections to out.
Assumes sorted_projections contains all global projections sorted.
Used by the naive_get_center implementation.
*/
void naive_copy_median_projection(double **sorted_projections, double *out) {
    if(n_points_global % 2) {
        /* is odd */
        long middle = (n_points_global - 1) / 2;
        copy_point(sorted_projections[middle] , out);
    }
    else {
        /* is even */
        long first_middle = (n_points_global / 2) - 1;
        long second_middle = (n_points_global / 2);
        middle_point(sorted_projections[first_middle], sorted_projections[second_middle], out);
    }
}

/*
Naive get_center implementation where all processes receive and sort all projections and copy the median to out.
Used when n_points_global < n_procs^2 and so parallel sorting by regular sampling is not viable.
*/
void mpi_naive_get_center(double *out) {
    int recv_counts[n_procs];
    int displays[n_procs];

    naive_compute_receive_info(recv_counts, displays);

    /*
    create a buffer to receive all the points
    and a separate buffer to sort all the points so that
    it is possible to free the memory at the end
    */
    double **receive_buffer = create_array_pts(n_dims, n_points_global);
    double **sorted_projections = (double**) malloc(sizeof(double*) * n_points_global);
    memcpy(sorted_projections, receive_buffer, sizeof(double*) * n_points_global);

    /* receive in sort_receive_buffer the orthogonal projections owned by all the processes */
    MPI_Allgatherv(
                *ortho_array,           /* address of what is being sent by the current process */
                recv_counts[rank],      /* how many elements are being sent */
                MPI_DOUBLE,             /* sending elements of type double */
                *receive_buffer,        /* address where I am receiving incoming data */
                recv_counts,            /* array stating how much data I will receive from each process*/
                displays,               /* displacement of data received by each process */
                MPI_DOUBLE,             /* receiving elements of type double */
                communicator            /* sending and receiving from/to the entire current team */
    );

    /* sort all orthogonal projections */
    qsort(sorted_projections, n_points_global, sizeof(double*), compare_point);

    naive_copy_median_projection(sorted_projections, out);

    /*free memory */
    free(sorted_projections);
    free(*receive_buffer);
    free(receive_buffer);
}
