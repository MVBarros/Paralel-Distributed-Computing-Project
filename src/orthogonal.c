#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gen_points.h"
#include "orthogonal.h"

extern double** pts; // point list
extern int n_dims; // number of dimensions of each point
extern long n_points; //number of points in the list
extern int root;

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

/*
* Print point p to stdout
*/
void print_point(double* p) {
    for(int i = 0; i < n_dims; i++){
           printf("%.2f ", p[i]);
    }
    printf("\n");
}

/*
* Returns the multiplication of value b by point a
*/
double* mul_scalar(double* a, double b){

    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] * b;
    }
    return c;
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
* Returns the sum of points a and b
*/
double* sum_points(double* a, double* b){
    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] + b[i];
    }
    return c;
}

/*
* Returns the difference of points a and b
*/
double* sub_points(double* a, double* b){
    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] - b[i];
    }
    return c;
}

/*
* Returns the ortogonal projection of point p onto line ab
*/
double* orthogonal_projection(double* a, double* b, double* p){
    double* basub = sub_points(b, a);
    double* pasub = sub_points(p, a);
    double c = dot_product(pasub,basub);
    double d = dot_product(basub,basub);
    double e = c/d;
    double* f = mul_scalar(basub,e);
    return sum_points(f, a);
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