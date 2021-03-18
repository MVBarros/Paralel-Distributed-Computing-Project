#include <stdio.h>
#include <stdlib.h>
#include "gen_points.h"
#include <omp.h>
#include <math.h>


double** pts;
int n_dims;
long n_points;
int root;


double distance(double *pt1, double *pt2)
{
    double dist = 0.0;

    for(int d = 0; d < n_dims; d++)
        dist += (pt1[d] - pt2[d]) * (pt1[d] - pt2[d]);
    return sqrt(dist);
}


int build_tree(){
    return 0;
}



int main(int argc, char** argv) {
    double exec_time;
    exec_time = -omp_get_wtime();
    pts = get_points(argc, argv, &n_dims, &n_points);
    root = build_tree();
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.1lf\n", exec_time);
    //dump_tree(root); 
}

