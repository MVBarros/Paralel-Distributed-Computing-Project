#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "gen_points.h"
#include "point_operations.h"
#include "ball_tree.h"

int n_dims; // number of dimensions of each point
node_ptr root;

double **pts, **ortho_array;
long n_points;

double *basub, *ortho_tmp;

#define RIGHT_PARTITION_SIZE() n_points % 2 ? (n_points + 1) % 2 : n_points % 2
#define LEFT_PARTITION_SIZE() n_points % 2 ? (n_points - 1) % 2 : n_points % 2


/*
* Returns the point in pts furthest away from point p
*/
double* get_furthest_away_point(double* p){
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

double get_radius(double* center){

    double* a = get_furthest_away_point(center);

    return sqrt(distance(a, center));

}

int compare_node(const void* pt1, const void* pt2) {

    double* dpt1 = *((double**) pt1);    
    double* dpt2 = *((double**) pt2);

    if(dpt1[0] > dpt2[0]) {
        return 1;
    }
    else if(dpt1[0] < dpt2[0]) {
        return -1;
    }
    else {
        return 0;
    }    
}

double* get_center() {
    qsort(ortho_array, n_points, sizeof(double*), compare_node);

    printf("Sorted ortogonal projections:\n");
    print_point_list(ortho_array);

    if(n_points % 2 == 0) { // is even
        long middle_1 = (n_points / 2) - 1;
        long middle_2 = (n_points / 2);
        
        return middle_point(ortho_array[middle_1], ortho_array[middle_2]);        
    }
    else { // is odd
        long middle = (n_points - 1) / 2;
        return copy_point(ortho_array[middle]); //copy since we will reuse ortho_array in future iterations of the algorithm
    }
}

void calc_orthogonal_projections(double* a, double* b) {
    sub_points(b, a, basub);
    for(long i = 0; i < n_points; i++){
        orthogonal_projection(basub, a, pts[i], ortho_array[i]);
    }
}

node_ptr build_tree(){

    double* a = get_furthest_away_point(pts[0]);

    print_point(a);
    
    double* b = get_furthest_away_point(a);

    print_point(b);

    calc_orthogonal_projections(a, b);
    
    printf("Ortogonal projections:\n");
    print_point_list(ortho_array);

    double* center = get_center();
    double radius = get_radius(center);

    printf("The center is: ");
    print_point(center);

    printf("The radius is: %f\n", radius);
    
    long right_size = RIGHT_PARTITION_SIZE();

    double **right = (double**) malloc(sizeof(double*) * right_size);

    long left_size = LEFT_PARTITION_SIZE();

    double **left = (double**) malloc(sizeof(double*) * left_size);

    return new_node(0, center, radius);
}

void alloc_memory() {
    ortho_array = create_array_pts(n_dims, n_points);
    basub = (double*) malloc(sizeof(double) * n_dims);
    ortho_tmp = (double*) malloc(sizeof(double) * n_dims);
}

int main(int argc, char** argv) {
    double exec_time;
    exec_time = -omp_get_wtime();
    pts = get_points(argc, argv, &n_dims, &n_points);
    alloc_memory();
    root = build_tree();
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.8lf\n", exec_time);
    dump_tree(root); 
}

