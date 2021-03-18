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

void print_point(long index){
    double* point = pts[index];
    printf("the point of index %ld is: ", index);
    for(int i = 0; i < n_dims; i++){
           printf(" %.2f  ", point[i]);
    }
    printf("\n");
}


int build_tree(){

    long farawaypoint= getmaxdistanceforindex_x(0);

    print_point(farawaypoint);
    
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

