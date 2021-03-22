#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "gen_points.h"
#include "point_operations.h"
#include "ball_tree.h"

int n_dims; // number of dimensions of each point
node_ptr root;

double **pts, **ortho_array, **pts_aux;
long n_points;
long node_id;

double *basub, *ortho_tmp;

node_ptr node_list;
double** node_centers;
long n_nodes;


long right_partition_size(){
    if(n_points % 2 == 0){
        return n_points / 2;
    }else{
        return (n_points + 1) / 2;
    }
}

long left_partition_size(){
    if(n_points % 2 == 0){
        return n_points / 2;
    }else{
        return (n_points - 1) / 2;
    }
}


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

    if(n_points % 2 == 0) { // is even
        long middle_1 = (n_points / 2) - 1;
        long middle_2 = (n_points / 2);
        
        middle_point(ortho_array[middle_1], ortho_array[middle_2], node_centers[node_id]);        
    }
    else { // is odd
        long middle = (n_points - 1) / 2;
        copy_point(ortho_array[middle], node_centers[node_id]);
    }
    return node_centers[node_id];
}

void calc_orthogonal_projections(double* a, double* b) {
    sub_points(b, a, basub);
    for(long i = 0; i < n_points; i++){
        orthogonal_projection(basub, a, pts[i], ortho_array[i]);
    }
}

void fill_partitions(double** left, double** right, double* center, double* a, double* b) {
    long l = 0;
    long r = 0;
    calc_orthogonal_projections(a, b);
    for(long i = 0; i < n_points; i++) {
        if(ortho_array[i][0] < center[0]) {            
            left[l] = pts[i];
            l++;
        }
        else {
            right[r] = pts[i];
            r++;
        }
    }
}

node_ptr build_tree(){
    if(n_points==1) {
        return make_node(node_id, pts[0], 0, &node_list[node_id]);
    }    

    double* a = get_furthest_away_point(pts[0]);
    double* b = get_furthest_away_point(a);

    calc_orthogonal_projections(a, b);

    double* center = get_center();
    double radius = get_radius(center);

    long n_points_left = left_partition_size();
    long n_points_right = right_partition_size();

    double **left = pts_aux;
    double **right = pts_aux + n_points_left;

    fill_partitions(left, right, center, a, b);

    node_ptr node = make_node(node_id, center, radius, &node_list[node_id]);

    double **child_pts_aux = pts;

    pts = left;
    pts_aux = child_pts_aux;    
    n_points = n_points_left;
    node_id++;
    node_ptr left_node = build_tree();

    pts = right;
    pts_aux = child_pts_aux;    
    n_points = n_points_right;
    node_id++;
    node_ptr right_node = build_tree();

    
    node->left_id = left_node->id;
    node->right_id = right_node->id;

    return node;
}

void alloc_memory() {
    n_nodes = (n_points * 2) - 1;
    ortho_array = create_array_pts(n_dims, n_points);
    basub = (double*) malloc(sizeof(double) * n_dims);
    ortho_tmp = (double*) malloc(sizeof(double) * n_dims);
    pts_aux = (double**) malloc(sizeof(double*) * n_points);
    node_list = (node_ptr) malloc(sizeof(node_t) * n_nodes);
    node_centers = create_array_pts(n_dims, n_nodes);
}

void dump_tree() {
    for (long i = 0; i < n_nodes; i++) {
        print_node(&node_list[i]);
    }
}

int main(int argc, char** argv) {
    double exec_time;
    exec_time = -omp_get_wtime();
    pts = get_points(argc, argv, &n_dims, &n_points);
    alloc_memory();
    root = build_tree();
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.8lf\n", exec_time);
    printf("%d %ld\n", n_dims, n_nodes);
    dump_tree(root); 
}