#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "gen_points.h"
#include "point_operations.h"

double** pts; // point list
int n_dims; // number of dimensions of each point
long n_points; //number of points in the list
int root;

/*
* Returns the point in pts furthest away from point p
*/
double* get_furthest_away_point(double* p, double** pts){
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

int build_tree(){

    double* a = get_furthest_away_point(pts[0], pts);

    print_point(a);
    
    double* b = get_furthest_away_point(a, pts);

    print_point(b);

    for(long i = 0; i < n_points; i++){
        print_point(orthogonal_projection(b, a, pts[i]));
    }
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

