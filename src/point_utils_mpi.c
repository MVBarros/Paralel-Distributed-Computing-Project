#include <stdio.h>
#include <mpi.h>
#include "point_operations.h"

extern long n_points_global;
extern long n_points_local;

extern int n_dims;

extern int rank;
extern int n_procs;

extern MPI_Comm communicator;

/*
Process root broadcast his local point at index i of pts to all other processes.
That point is copied onto out
*/
void mpi_broadcast_point(double **pts, long i, int root, double *out) {
    if(rank == root){
        /*send*/
        MPI_Bcast(
                pts[i],             /*the address of the data the current process is sending*/
                n_dims,             /*the number of data elements sent*/
                MPI_DOUBLE,         /*type of data elements sent*/
                root,               /*rank of the process sending the data*/
                communicator        /*broadcast to all processes in the current team*/
        );
        copy_point(pts[i], out);
    }
    else{
        /*receive*/
        MPI_Bcast(
                out,                /*the address of the data the current process is receiving*/
                n_dims,             /*the number of data elements to receive*/
                MPI_DOUBLE,         /*type of data elements received*/
                root,               /*rank of the process sending the data*/
                communicator        /*broadcast to all processes in the current team*/
        );
    }
}

/*
Get nth point in the global point list pts.
The distribution of points is given by processes_n_points.
The nth point is copied to out at all processes
*/
void mpi_get_point(double **pts, long n, long* processes_n_points, double* out) {
    long count = 0;
    long displacement = 0;
    for(int i = 0; i < n_procs; i++){
        if(processes_n_points[i] + count > n ){
            displacement = n - count;
            mpi_broadcast_point(pts, displacement, i, out);
            break;
        }
        else {
            count += processes_n_points[i];
        }
    }
}


/*
Puts in out the value of my_count at each process
*/
void mpi_get_processes_counts(long my_count, long *out) {

    /*Broadcast all-to-all of my_count value held locally*/
    MPI_Allgather(
                &my_count,              /*the address of the data the current process is sending*/
                1,                      /*number of data elements sent*/
                MPI_LONG,               /*type of data element sent*/
                out,                    /*the address where the current process stores the data received*/
                1,                      /*number of data elements sent by each process*/
                MPI_LONG,               /*type of data element received*/
                communicator            /*sending and receiving to all processes in the current team*/
    );
}