#ifndef POINT_OPERATIONS_H
#define POINT_OPERATIONS_H

double distance(double* pt1, double* pt2);

// Print point p to stdout
void print_point(double* p);

// Returns the multiplication of value b by point a
double* mul_scalar(double* a, double b);

//Returns the dot product of points a and b
double dot_product(double* a, double* b);

// Returns the sum of points a and b
double* sum_points(double* a, double* b);

//Returns the difference of points a and b
double* sub_points(double* a, double* b);

//Returns the ortogonal projection of point p onto line ab
double* orthogonal_projection(double* a, double* b, double* p);

#endif