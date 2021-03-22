#ifndef POINT_OPERATIONS_H
#define POINT_OPERATIONS_H

double distance(double* pt1, double* pt2);

// Print point p to stdout
void print_point(double* p);

// Print point list pt_list to stdout
void print_point_list(double** pt_list);

// Puts in out the multiplication of value b by point a
void mul_scalar(double* a, double b, double* out);

//Returns the dot product of points a and b
double dot_product(double* a, double* b);

//Puts in out the sum of points a and b
void sum_points(double* a, double* b, double* out);

//Puts in out the difference of points a and b
void sub_points(double* a, double* b, double* out);

//Puts in out the ortogonal projection of point p onto line starting in a and defined by basub
void orthogonal_projection(double* basub, double* b, double* p, double* out);

//Returns the middle of points a and b
void middle_point(double* a, double* b, double* out);

//Returns a copy of point p
void copy_point(double* p, double* out);

#endif