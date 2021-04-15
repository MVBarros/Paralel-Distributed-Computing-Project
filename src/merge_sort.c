#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>


//  Left source half is A[ iBegin:iMiddle-1].
// Right source half is A[iMiddle:iEnd-1   ].
// Result is            B[ iBegin:iEnd-1   ].
void TopDownMerge(double** A, long iBegin, long iMiddle, long iEnd, double** B)
{
    long i = iBegin, j = iMiddle;
 
    // While there are elements in the left or right runs...
    for (long k = iBegin; k < iEnd; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < iMiddle && (j >= iEnd || A[i][0] <= A[j][0])) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
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

// Split A[] into 2 runs, sort both runs into B[], merge both runs from B[] to A[]
// iBegin is inclusive; iEnd is exclusive (A[iEnd] is not in the set).
void TopDownSplitMerge(double** B, long iBegin, long iEnd, double** A, int depth_atm, int depth_max)
{
    if(iEnd - iBegin <= 1)                      // if run size == 1
        return;                                 //   consider it sorted
    // split the run longer than 1 item into halves
    long iMiddle = (iEnd + iBegin) / 2; 

    if(depth_atm < depth_max){
        depth_atm +=1;    
        TopDownSplitMerge(A, iBegin,  iMiddle, B, depth_atm, depth_max);  // sort the left  run
        TopDownSplitMerge(A, iMiddle,    iEnd, B, depth_atm, depth_max);  // sort the right run
        // merge the resulting runs from array B[] into A[]
        TopDownMerge(B, iBegin, iMiddle, iEnd, A);
    }else{
        qsort(A+iBegin, iEnd-iBegin, sizeof(double*), compare_node);
    }  

}


// Array A[] has the items to sort; array B[] is a work array.
void TopDownMergeSort(double** A, double** B, long n, int depth_max)
{
    memcpy(B, A, n* sizeof(double*));           // one time copy of A[] to B[]
    TopDownSplitMerge(B, 0, n, A, 0, depth_max);   // sort data from B[] into A[]
}
