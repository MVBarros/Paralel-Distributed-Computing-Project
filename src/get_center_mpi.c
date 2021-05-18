#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <mpi.h>
#include "point_operations.h"
#include "gen_points.h"
#include "macros.h"
#include "point_utils_mpi.h"

extern double **ortho_array;
extern double **ortho_array_srt;

extern double *median_left_point;
extern double *median_right_point;

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

/*
Copies the median projection to out.
The distribution of projections is given by processes_n_points
Used by the naive_get_center implementation.
*/
void mpi_psrs_copy_median_projection(double **sorted_projections, long *processes_n_points, double *out) {
    if(n_points_global % 2) {
        /* is odd */
        long middle = (n_points_global - 1) / 2;
        mpi_get_point(sorted_projections, middle, processes_n_points, out);
    }
    else {
        /* is even */
        long first_middle = (n_points_global / 2) - 1;
        long second_middle = (n_points_global / 2);
        mpi_get_point(sorted_projections, first_middle, processes_n_points, median_left_point);
        mpi_get_point(sorted_projections, second_middle, processes_n_points, median_right_point);
        middle_point(median_left_point, median_right_point, out);
    }
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
void mpi_psrs_gather_global_samples(double *local_samples, double *global_samples) {
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
Computes the pivots for the psrs algorithm by taking n_proc local regular samples of the
x coordinate of the orthogonal projections, gathering all local regular samples
and tanking n_proc -1 regular samples of the gathered result
*/
void mpi_psrs_get_pivots(double *pivots) {
    double local_samples[n_procs];
    double* global_samples = (double*)malloc(sizeof(double) * n_procs * n_procs);

    psrs_calc_local_samples(local_samples);
    mpi_psrs_gather_global_samples(local_samples, global_samples);

    qsort(global_samples, n_procs*n_procs , sizeof(double), compare_double);

    long step = n_procs;
    long n_pivots = n_procs - 1;
    for(long i = 0, j = n_procs; i < n_pivots; i++, j += step) {
        pivots[i] = global_samples[j];
    }

    free(global_samples);
}

/*
Copies the ortogonal projections present in ortho_array to ortho_array_srt and sorts ortho_array_srt
*/
void psrs_sort_local_projections() {
    memcpy(ortho_array_srt, ortho_array, sizeof(double*) * n_points_local);
    qsort(ortho_array_srt, n_points_local, sizeof(double*), compare_point);
}

/*
Places in send_counts how much data elements will be sent to each process and in send_displays
the displacement of said data.
Each process i receives the points before pivot at index i.
*/
void psrs_get_send_info(double* pivots, int *send_counts, int *send_displays){
    memset(send_counts, 0, sizeof(int) * n_procs);
    long j = 0;
    long k = 0;
    for(long i = 0; i < n_points_local && j != n_procs - 1 ;i++){
        if (ortho_array_srt[i][0] > pivots[j]){
            send_counts[j] = i - k;
            j++;
            k = i;
        }
    }
    send_counts[j] = n_points_local - k;

    int count = 0;
    for(int i = 0; i < n_procs; i++){
        send_counts[i] *= n_dims;
        send_displays[i] = count;
        count += send_counts[i];
    }
}

/*
Places in receive_counts how much data elements the current process will receive from each process and in receive_displays
the displacement of said data.
Returns the total ammount of points received.
*/
long psrs_get_receive_info(int *send_counts, int *receive_counts, int *receive_displays){
    /* Broadcast all-to-all each process sends to each process what is should sent him */
    MPI_Alltoall(
                send_counts,            /* send how much I will send each process */
                1,                      /* send one value to each process */
                MPI_INT,                /* value of type int */
                receive_counts,         /* store how much i should receive in receive_counts */
                1,                      /* receive one value from each process */
                MPI_INT,                /* value of type int */
                communicator            /* sending and receiving to all processes in the current team */
    );

    int count = 0;
    for(int i = 0; i < n_procs; i++){
        receive_displays[i] = count;
        count += receive_counts[i];
    }

    return (receive_displays[n_procs - 1] + receive_counts[n_procs - 1]) / n_dims;
}

/*
Transfers to each process his partition of the points.
Process i receives all the points smaller than pivot i from all other processes.
Returns a pointer to the received data.
*/
double** mpi_psrs_exchange_projections(int *send_counts, int *send_displays, int *receive_counts, int *receive_displays, long n_points_receive) {
    double **send_buffer = create_array_pts(n_dims, n_points_local);
    double **receive_buffer = create_array_pts(n_dims, n_points_receive);

    copy_point_list(ortho_array_srt, send_buffer, n_points_local);

    MPI_Alltoallv(
                *send_buffer,       /* starting address of the data to send*/
                send_counts,        /* array stating for each process i the number of data elements the current process will send them */
                send_displays,      /* array stating for each process i the displacement of the data sent to them in send_buffer */
                MPI_DOUBLE,         /* sending data elements of type double */
                *receive_buffer,    /* starting address of where incoming data will be received */
                receive_counts,     /* array stating for each process i the number of data elements the current process will receive from them */
                receive_displays,   /* array stating for each process i the displacement of the data received from them in recv_buffer */
                MPI_DOUBLE,         /* receiving data elements of type double */
                communicator        /* sending and receiving to all processes in the current team */
    );

    free(*send_buffer);
    free(send_buffer);

    return receive_buffer;
}

/*
Normalizes receive_counts and receive_displays from storing the number of data elements
to storing the number of points received.
*/
void psrs_normalize_receive_info(int *receive_counts, int *receive_displays) {
    for(int i = 0; i < n_procs; i++) {
        receive_counts[i] = receive_counts[i] / n_dims;

    }
    for(int i = 0; i < n_procs; i++) {
        receive_displays[i] = receive_displays[i] / n_dims;
    }
}
/*
Place into receive_processes_n_points the number of points received at each process (n_points_receive)
Asynchronous operation. The algorithm does useful work while it's receiving (namely merging the received points)
*/
void mpi_async_psrs_gather_n_points_receive(long *receive_processes_n_points ,long n_points_receive, MPI_Request *request){
    MPI_Iallgather(
                &n_points_receive,          /* address of sent data */
                1,                          /* number of data elements to send */
                MPI_LONG,                   /* sent elements of type long */
                receive_processes_n_points, /* adress where received data is stored */
                1,                          /* number of data elements received from each process */
                MPI_LONG,                   /* receive elements of type long */
                communicator,               /* communicate with entire team */
                request                     /* place in request the asynchronous request */
    );
}

/*
Sorts point list points of size n that is split in p sorted partitions whose size and displacement
is given by counts and displays and writes the result in out
*/
void psrs_merge_sorted_point_list_partitions(double **points, int *receive_counts, int *receive_displays, long n, double **out) {
    psrs_normalize_receive_info(receive_counts, receive_displays);
    int receive_indexes[n_procs];
    memset(receive_indexes, 0, n_procs * sizeof(int));

    for(long i = 0; i < n; i++) {
        double min = DBL_MAX;
        double *point = NULL;
        int k = 0;
        for(int j = 0; j < n_procs; j++) {
            if (receive_counts[j] == receive_indexes[j]) {
                continue;
            }
            long l = receive_displays[j] + receive_indexes[j];
            if (points[l][0] < min) {
                point = points[l];
                min = point[0];
                k = j;
            }
        }
        out[i] = point;
        receive_indexes[k]++;
    }
}

/*
get_center implementation that uses parallel sorting by regular sampling
to sort orthogonal projections and copies the median to out.
Assumes that n_points_global >= n_procs^2.
*/
void mpi_psrs_get_center(double* out) {
    double pivots[n_procs - 1];

    int receive_counts[n_procs];
    int receive_displays[n_procs];

    int send_counts[n_procs];
    int send_displays[n_procs];

    long receive_processes_n_points[n_procs];

    MPI_Request request;

    psrs_sort_local_projections();
    mpi_psrs_get_pivots(pivots);

    psrs_get_send_info(pivots, send_counts, send_displays);

    long n_points_receive = psrs_get_receive_info(send_counts, receive_counts, receive_displays);

    double **receive_buffer = mpi_psrs_exchange_projections(send_counts, send_displays, receive_counts, receive_displays, n_points_receive);

    mpi_async_psrs_gather_n_points_receive(receive_processes_n_points, n_points_receive, &request);

    /* sort received projections while gathering receive_processes_n_points asynchronously */
    double **sorted_projections = (double**) malloc(sizeof(double*) * n_points_receive);
    psrs_merge_sorted_point_list_partitions(receive_buffer, receive_counts, receive_displays, n_points_receive, sorted_projections);

    MPI_Wait(&request, MPI_STATUS_IGNORE);

    mpi_psrs_copy_median_projection(sorted_projections, receive_processes_n_points, out);

    free(*receive_buffer);
    free(receive_buffer);
    free(sorted_projections);
}