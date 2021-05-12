#ifndef GEN_POINTS_MPI_H
#define GEN_POINTS_MPI_H

double **create_array_pts(int n_dims, long np);

double **get_points(int argc, char *argv[], int *n_dims, long *np);

void free_array_pts(double ** p_arr);

long realoc_array_pts(double ** p_arr, int n_dims, long curr_size, long new_size);

#endif