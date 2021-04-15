#ifndef MERGE_SORT_H
#define MERGE_SORT_H


void TopDownMergeSort(double** A, double** B, long n);

void TopDownSplitMerge(double** B, long iBegin, long iEnd, double** A);

void TopDownMerge(double** A, long iBegin, long iMiddle, long iEnd, double** B);

#endif