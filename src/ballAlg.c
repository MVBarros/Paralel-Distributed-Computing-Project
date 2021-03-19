#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "gen_points.h"
#include "orthogonal.h"

double** pts; // point list
int n_dims; // number of dimensions of each point
long n_points; //number of points in the list
int root;

int main(int argc, char** argv) {
    double exec_time;
    exec_time = -omp_get_wtime();
    pts = get_points(argc, argv, &n_dims, &n_points);
    root = build_tree();
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.1lf\n", exec_time);
    //dump_tree(root); 
}

