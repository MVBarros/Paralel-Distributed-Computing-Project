#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "gen_points.h"
#include "point_operations.h"
#include "ball_tree.h"

int n_dims; // number of dimensions of each point

double **pts; // list of points of the current iteration of the algorithm
double **ortho_array; // list of ortogonal projections of the points in pts
double **ortho_array_srt; //list of ortogonal projections of the point in pts to be sorted.
double **pts_aux; // list of points of the next iteration of the algorithm

long n_points; //number of points in the dataset

double  *basub; // point containing b-a for the orthogonal projections
double *ortho_tmp; // temporary pointer used for calculation the orthogonal projection

node_ptr node_list; // list of nodes of the ball tree
double** node_centers; // list of centers of the ball tree nodes

long n_nodes; // number of nodes of the ball tree
long node_id; // id of the current node of the algorithm
long node_counter; // number of nodes generated by the program

#define LEFT_PARTITION_SIZE(N) ((N) % 2 ? ((N) - 1) / 2 : (N) / 2)
#define RIGHT_PARTITION_SIZE(N) ((N) % 2 ? ((N) + 1) / 2 : (N) / 2)

/*
Returns the point in pts furthest away from point p
*/
double* get_furthest_away_point(double* p) {
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

/*
Returns the radius of the ball tree node defined by point center
*/
double get_radius(double* center) {
    double* a = get_furthest_away_point(center);
    return sqrt(distance(a, center));
}

/*
Returns the median projection of the dataset
by sorting the projections based on their x coordinate
*/
double* get_center() {
    memcpy(ortho_array_srt, ortho_array, sizeof(double*) * n_points);
    qsort(ortho_array_srt, n_points, sizeof(double*), compare_point);

    if(n_points % 2) { // is odd
        long middle = (n_points - 1) / 2;
        copy_point(ortho_array_srt[middle], node_centers[node_counter]);
    }
    else { // is even
        long first_middle = (n_points / 2) - 1;
        long second_middle = (n_points / 2);

        middle_point(ortho_array_srt[first_middle], ortho_array_srt[second_middle], node_centers[node_counter]);
    }
    return node_centers[node_counter];
}

/*
Computes the orthogonal projections of points in pts onto line defined by b-a
*/
void calc_orthogonal_projections(double* a, double* b) {
    sub_points(b, a, basub);
    for(long i = 0; i < n_points; i++){
        orthogonal_projection(basub, a, pts[i], ortho_array[i], ortho_tmp);
    }
}

/*
Places each point in pts in partition left or right by comparing the x coordinate
of its orthogonal projection with the x coordinate of the center point
*/
void fill_partitions(double** left, double** right, double* center) {
    long l = 0;
    long r = 0;
    for(long i = 0; i < n_points; i++) {
        if(ortho_array[i][0] < center[0]) {
            copy_point(pts[i], left[l]);
            l++;
        }
        else {
            copy_point(pts[i], right[r]);
            r++;
        }
    }
}

void build_tree() {
    if(n_points == 1) {
        copy_point(pts[0], node_centers[node_counter]);
        make_node(node_id, node_centers[node_counter], 0, &node_list[node_counter]);
        node_counter++;
        return;
    }

    double* a = get_furthest_away_point(pts[0]);
    double* b = get_furthest_away_point(a);

    calc_orthogonal_projections(a, b);

    double* center = get_center();
    double radius = get_radius(center);

    node_ptr node = make_node(node_id, center, radius, &node_list[node_counter]);
    node_counter++;

    double **left = pts_aux;
    double **pts_aux_left = pts;
    double **ortho_array_left = ortho_array;
    double **ortho_array_srt_left = ortho_array_srt;
    long n_points_left = LEFT_PARTITION_SIZE(n_points);

    double **right = pts_aux + n_points_left;
    double **pts_aux_right = pts + n_points_left;
    double **ortho_array_right = ortho_array + n_points_left;
    double **ortho_array_srt_right = ortho_array_srt + n_points_left;
    long n_points_right = RIGHT_PARTITION_SIZE(n_points);

    long node_id_left = 2 * node_id + 1;
    long node_id_right = 2 * node_id + 2;

    fill_partitions(left, right, center);

    pts = left;
    pts_aux = pts_aux_left;
    ortho_array = ortho_array_left;
    ortho_array_srt = ortho_array_srt_left;
    n_points = n_points_left;
    node_id = node_id_left;
    build_tree();

    pts = right;
    pts_aux = pts_aux_right;
    ortho_array = ortho_array_right;
    ortho_array_srt = ortho_array_srt_right;
    n_points = n_points_right;
    node_id = node_id_right;
    build_tree();

    node->left_id = node_id_left;
    node->right_id = node_id_right;
}

void alloc_memory() {
    n_nodes = (n_points * 2) - 1;
    ortho_array = create_array_pts(n_dims, n_points);
    ortho_array_srt = (double**) malloc(sizeof(double*) * n_points);
    basub = (double*) malloc(sizeof(double) * n_dims);
    ortho_tmp = (double*) malloc(sizeof(double) * n_dims);
    pts_aux = create_array_pts(n_dims, n_points);
    node_list = (node_ptr) malloc(sizeof(node_t) * n_nodes);
    node_centers = create_array_pts(n_dims, n_nodes);
}

int main(int argc, char** argv) {
    double exec_time;
    exec_time = -omp_get_wtime();
    pts = get_points(argc, argv, &n_dims, &n_points);
    alloc_memory();
    build_tree();
    exec_time += omp_get_wtime();
    fprintf(stderr, "%.1lf\n", exec_time);
    printf("%d %ld\n", n_dims, n_nodes);
    dump_tree();
}