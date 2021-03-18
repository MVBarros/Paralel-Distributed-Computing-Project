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

long getmaxdistanceforindex_x(long index){
    double max = 0.0;
    long indexmax = index;
    for(int p = 0; p < n_points; p++){
        if(distance(pts[index], pts[p]) > max){
            max = distance(pts[index], pts[p]);
            indexmax = p;
        }        
    }
    return indexmax;
}

void print_index(long index){
    double* point = pts[index];
    printf("the point of index %ld is: ", index);
    for(int i = 0; i < n_dims; i++){
           printf(" %.2f  ", point[i]);
    }
    printf("\n");
}

void print_point(double* point){
    for(int i = 0; i < n_dims; i++){
           printf(" %.2f  ", point[i]);
    }
    printf("\n");
}

double* mult_point(double* a, double* b){

    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] * b[i];
    }
    return c;
}

double* mult_scalar(double* a, double b){

    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] * b;
    }
    return c;
}

double dotproduct_points(double* a, double* b){
    double c = 0;
    for(int i = 0; i < n_dims; i++){
        c += a[i] * b[i];
    }
    return c;
}



double* sum_points(double* a, double* b){
    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] + b[i];
    }
    return c;
}

double* sub_points(double* a, double* b){
    double* c = (double*) malloc(sizeof(double)*n_dims);
    for(int i = 0; i < n_dims; i++){
        c[i] = a[i] - b[i];
    }
    return c;
}


/*
po =
(p − a) · (b − a)
(b − a) · (b − a)
(b − a) + a
*/

double* orthogonal(long a, long b, long p){
    double* basub = sub_points(pts[b],pts[a]);
    double* pasub = sub_points(pts[p],pts[a]);
    double c = dotproduct_points(pasub,basub);
    double d = dotproduct_points(basub,basub);
    double e = c/d;
    double* f = mult_scalar(basub,e);
    return sum_points(f, pts[a]);

}




int build_tree(){

    long a= getmaxdistanceforindex_x(0);

    print_index(a);
    
    long b = getmaxdistanceforindex_x(a);

    print_index(b);

    for(long i = 0; i < n_points; i++){
        print_point(orthogonal(b,a,i));
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

