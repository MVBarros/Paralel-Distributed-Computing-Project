#include <stdlib.h>
#include <stdio.h>
#include "ball_tree.h"
#include "point_operations.h"

node_ptr make_node(long id, double* center, double radius, node_ptr new_node) {
    new_node->radius = radius;
    new_node->center = center;
    new_node->id = id;
    new_node->left = NULL;
    new_node-> right = NULL;
    return new_node;
}

void print_node(node_ptr node) {
    if (node == NULL) {
        return;
    }
    printf("%ld ", node->id);
    if (node->left == NULL) {
        printf("-1 ");
    } else {
        printf("%ld ", node->left->id);
    }

    if (node->right == NULL ) {
        printf("-1 ");
    } else {
        printf("%ld ", node->right->id);
    }

    printf("%.6f", node->radius);
    print_point(node->center);
}