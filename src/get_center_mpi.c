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
extern double **ortho_array_srt;

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

/*
Calculate n_proc regular samples of the x coordenated of ortho_array_srt and place them in samples.
Assumes n_points_local >= n_proc
*/
void psrs_calc_local_samples(double *local_samples) {
    long step = n_points_local / n_procs;
    long j = 0;
    for(int i = 0; i < n_procs; i++, j += step) {
        local_samples[i] = ortho_array_srt[j][0];
    }
}

/*
Place into global_samples the samples gathered at each process (local_samples)
*/
void mpi_psrs_gather_all_samples(double *local_samples, double *global_samples) {
    MPI_Allgather(
                local_samples,      /* send my local regular samples */
                n_procs,            /* measured n_proc regular samples */
                MPI_DOUBLE,         /* samples of type double */
                global_samples,     /* receive all regular samples in global_samples */
                n_procs,            /* receive n_proc samples from each process */
                MPI_DOUBLE,         /* samples of type double */
                communicator        /* send and receive from the whole team */
    );
}

/*
Calculate n_proc - 1 pivots of global_samples and places them in pivots
*/
void psrs_get_pivots(double *global_samples, double *pivots){
    qsort(global_samples, n_procs*n_procs , sizeof(double), compare_double);

    //TODO get pivots
}

/*
get_center implementation that uses parallel sorting by regular sampling
to sort orthogonal projections and copies the median to out.
Assumes that n_points_global >= n_procs^2.
*/
void mpi_psrs_get_center(){
    double local_samples[n_procs];
    double pivots[n_procs - 1];

    double* global_samples = (double*)malloc(sizeof(double) * n_procs * n_procs);

    /* order local projections */
    memcpy(ortho_array_srt, ortho_array, sizeof(double*) * n_points_local);
    qsort(ortho_array_srt, n_points_local, sizeof(double*), compare_point);

    psrs_calc_local_samples(local_samples);

    mpi_psrs_gather_all_samples(local_samples, global_samples);

    psrs_get_pivots(global_samples, pivots);


}