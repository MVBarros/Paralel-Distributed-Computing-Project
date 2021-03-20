#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "gen_points.h"
#include "point_operations.h"
#include "ball_tree.h"

int n_dims; // number of dimensions of each point
node_ptr root;

/*
* Returns the point in pts furthest away from point p
*/
double* get_furthest_away_point(double* p, double** pts, long n_points){
    double max_distance = 0.0;
    double* furthest_point = p;
    for(long i = 0; i < n_points; i++){
        double curr_distance = distance(p, pts[i]);
        if(curr_distance > max_distance){
            max_distance = curr_distance;
            furthest_point = pts[i];
        }        
    }
    return furthest_point;
}

node_ptr build_tree(double** pts, long n_points){

    double* a = get_furthest_away_point(pts[0], pts, n_points);

    print_point(a);
    
    double* b = get_furthest_away_point(a, pts, n_points);

    print_point(b);

    for(long i = 0; i < n_points; i++){
        print_point(orthogonal_projection(b, a, pts[i]));
    }
    return NULL;
}

int main(int argc, char** argv) {
    double exec_time;
    long n_points;
    exec_time = -omp_get_wtime();
    double **pts = get_points(argc, argv, &n_dims, &n_points);
    root = build_tree(pts, n_points);
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.1lf\n", exec_time);
    dump_tree(root); 
}

