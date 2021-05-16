#ifndef POINT_UTILS_MPI_H
#define POINT_UTILS_MPI_H

void mpi_get_point(double **pts, long n, long* processes_n_points, double* out);

void mpi_get_processes_counts(long my_count, long *out);
#endif