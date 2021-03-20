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

int compare_node(const void* pt1, const void* pt2) {

    double* dpt1 = *((double**) pt1);    
    double* dpt2 = *((double**) pt2);

    if(dpt1[0] > dpt2[0]) {
        return 1;
    }
    else if(dpt1[0] == dpt2[0]) {
        return 0;
    }
    else {
        return -1;
    }    
}



double* get_median(double** pts, long n_points) {
    // is even
    if(n_points % 2 == 0) {
        long middle_1 = (n_points / 2) - 1;
        long middle_2 = (n_points / 2);
        
        return middle_points(pts[middle_1], pts[middle_2]);        
    }
    // is odd
    else {
        long middle = (n_points - 1) / 2;
        return pts[middle];
    }
}

node_ptr build_tree(double** pts, long n_points){

    double* a = get_furthest_away_point(pts[0], pts, n_points);

    print_point(a);
    
    double* b = get_furthest_away_point(a, pts, n_points);

    print_point(b);

    double** ortho_array = (double**) malloc(sizeof(double*) * n_points);

    printf("Projeccao ortogonal dos pontos:\n");
    for(long i = 0; i < n_points; i++){
        ortho_array[i] = orthogonal_projection(b, a, pts[i]);
        print_point(ortho_array[i]);
    }
    qsort(ortho_array, n_points, sizeof(double*), compare_node);

    printf("Pontos apos ordenacao:\n");
    for(long i = 0; i < n_points; i++) {
        print_point(ortho_array[i]);
    }

    double* middle = get_median(ortho_array, n_points);

    printf("The middle point is: ");
    print_point(middle);

    return NULL;
}

int main(int argc, char** argv) {
    double exec_time;
    long n_points;
    exec_time = -omp_get_wtime();
    double **pts = get_points(argc, argv, &n_dims, &n_points);
    root = build_tree(pts, n_points);
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.8lf\n", exec_time);
    dump_tree(root); 
}

