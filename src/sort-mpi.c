#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include "point_operations.h"
#include "sort-mpi.h"
#include "macros.h"

extern double ** ortho_array;
extern double **sorted_projections;
extern double **sort_receive_buffer;

extern long n_points_local;
extern long n_dims;
extern long n_points_global;

extern long *processes_n_points;

extern int rank;
extern int n_procs;

/*
Placeholder sort where everyone receives and sorts all points
*/
double **mpi_projections_sort() {
    int recv_counts[n_procs];
    int displays[n_procs];

    int displacement = 0;
    int my_displacement = 0;

    for(int i = 0; i < n_procs; i++) {
        if (rank == i) {
            my_displacement = displacement;
        }
        recv_counts[i] = processes_n_points[i] * n_dims; //receiving processes_n_points[i] * n_dims double from process i
        displays[i] = displacement * n_dims; //place data received from process i starting in index display
        displacement += processes_n_points[i];
    }

    /* put in sort_receive_buffer all the orthogonal projections of all the points of all the processes */
    MPI_Allgatherv(
                *ortho_array,           /* address of what is being sent by the current process */
                recv_counts[rank],      /* how many elements are being sent */
                MPI_DOUBLE,             /* sending elements of type double */
                *sort_receive_buffer,   /* address where I am receiving incoming data */
                recv_counts,            /* array stating how much data I will receive from each process*/
                displays,               /* displacement of data received by each process */
                MPI_DOUBLE,             /* receiving elements of type double */
                MPI_COMM_WORLD          /* communicating with the entire world */
    );

    copy_point_list(sort_receive_buffer, sorted_projections, n_points_global);
    qsort(sorted_projections, n_points_global, sizeof(double*), compare_point);

    /*TODO parallel distributed sort*/
    return &sorted_projections[my_displacement];
}
