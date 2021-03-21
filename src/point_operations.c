#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gen_points.h"
#include "point_operations.h"

extern int n_dims; // number of dimensions of each point
extern int n_points;

extern double* ortho_tmp;

/*
* Returns the squared distance between points pt1 and pt2
*/
double distance(double* pt1, double* pt2)
{
    double dist = 0.0;

    for(int i = 0; i < n_dims; i++)
        dist += (pt1[i] - pt2[i]) * (pt1[i] - pt2[i]);
    return dist; 
}

/*
* Print point p to stdout
*/
void print_point(double* p) {
    for(int i = 0; i < n_dims; i++){
        printf(" %.6f", p[i]);
    }
    printf("\n");
}

/*
* Print point list pt_list to stdout
*/
void print_point_list(double** pt_list) {
    for (long i = 0; i < n_points; i++) {
        print_point(pt_list[i]);
    }
}


/*
* Puts in out multiplication of value b by point a
*/
void mul_scalar(double* a, double b, double* out){
    for(int i = 0; i < n_dims; i++){
        out[i] = a[i] * b;
    }
}

/*
* Returns the dot product of points a and b
*/
double dot_product(double* a, double* b){
    double c = 0;
    for(int i = 0; i < n_dims; i++){
        c += a[i] * b[i];
    }
    return c;
}

/*
* Puts in out the sum of points a and b
*/
void sum_points(double* a, double* b, double* out){
    for(int i = 0; i < n_dims; i++){
        out[i] = a[i] + b[i];
    }
}

/*
* Puts in out the difference of points a and b
*/
void sub_points(double* a, double* b, double* out){
    for(int i = 0; i < n_dims; i++){
        out[i] = a[i] - b[i];
    }
}

/*
* Returns a copy of point p
*/
double* copy_point(double* p) {
    double* copy = malloc(sizeof(double) * n_dims);
    for(int i = 0; i < n_dims; i++){
        copy[i] = p[i];
    }
    return copy;
}

/*
* Puts in out the ortogonal projection of point p onto line starting in a and defined by basub
*/
void orthogonal_projection(double* basub, double* a , double* p, double* out){
    sub_points(p, a, ortho_tmp);
    double c = dot_product(ortho_tmp,basub);
    double d = dot_product(basub,basub);
    double e = c/d;
    mul_scalar(basub,e, ortho_tmp);
    sum_points(ortho_tmp, a, out);
}

/*
* Returns the middle of points a and b
*/
double* middle_point(double* a, double* b){
    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = (a[i] + b[i]) / 2;
    }
    return c;
}