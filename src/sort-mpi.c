#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "point_operations.h"
#include "sort-mpi.h"

extern double ** ortho_array;
extern double **ortho_array_srt;

extern long n_points_local;
extern long n_dims;
extern long n_points_global;

double **mpi_projections_sort() {
    memcpy(ortho_array_srt, ortho_array, sizeof(double*) * n_points_local);
    qsort(ortho_array_srt, n_points_local, sizeof(double*), compare_point);

    /*TODO parallel distributed sort*/
    return NULL;
}
