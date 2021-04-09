#include <stdlib.h>
#include <stdio.h>
#include "ball_tree.h"
#include "point_operations.h"

extern int n_nodes;
extern node_ptr node_list;


node_ptr make_node(long id, double* center, double radius, node_ptr new_node) {
    new_node->radius = radius;
    new_node->center = center;
    new_node->id = id;
    new_node->left_id = -1;
    new_node->right_id = -1;
    return new_node;
}

void print_node(node_ptr node) {

    printf("%ld %ld %ld %.6f", node->id, node->left_id, node->right_id, node->radius);

    print_point(node->center);
}

void dump_tree() {
    for (long i = 0; i < n_nodes; i++) {
        print_node(&node_list[i]);
    }
}