#ifndef BALL_TREE_H
#define BALL_TREE_H

struct tree_node {
    long id;
    double radius;
    double* center;
    struct tree_node *left;
    struct tree_node *right;
};

typedef struct tree_node node_t;
typedef struct tree_node *node_ptr;

node_ptr new_node(long id, double* center, double radius);
void dump_tree(node_ptr node);

#endif