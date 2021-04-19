#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>

/*
Left source half is a[ begin:middle-1].
Right source half is a[middle:end-1   ].
Result is            b[ begin:end-1   ].
*/
void top_down_merge(double** a, long begin, long middle, long end, double** b) {
    long i = begin, j = middle;

    // While there are elements in the left or right runs...
    for (long k = begin; k < end; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < middle && (j >= end || a[i][0] <= a[j][0])) {
            b[k] = a[i];
            i = i + 1;
        } else {
            b[k] = a[j];
            j = j + 1;
        }
    }
}

/*
Used for quicksort
Compares the x coordenate of the two points
*/
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

/*
Split a into 2 runs, sort both runs into b, merge both runs from b to a
*/
void top_down_split_merge(double** b, long begin, long end, double** a, int depth_atm, int depth_max) {
    if(end - begin <= 1)
        return; //already sorted
    int x =0, y = 0;
    if(depth_atm < depth_max){
        long middle = (end + begin) / 2;
        #pragma omp task
        {
            top_down_split_merge(a, begin,  middle, b, depth_atm + 1, depth_max);  // sort the left run
        }
        #pragma omp task
        {
            top_down_split_merge(a, middle,    end, b, depth_atm + 1, depth_max);  // sort the right run

        }
        // merge the resulting runs from array b into a
        #pragma omp taskwait
        {
            top_down_merge(b, begin, middle, end, a);
        }      
    } else {
        qsort(a+begin, end-begin, sizeof(double*), compare_node);
    }
}

/*
Array a has the items to sort; array b is a work array.
*/
void merge_sort(double** a, double** b, long n, int depth_max) {
    memcpy(b, a, n* sizeof(double*));           // one time copy of a[] to b[]
    top_down_split_merge(b, 0, n, a, 0, depth_max);   // sort data from b[] into a[]
}
