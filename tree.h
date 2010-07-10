#ifndef TREE_H
#define TREE_H

#include "stack.h"
#include "types.h"
#include "sl_list.h"

double expandbox(SL_LIST *body_list, VECTOR c);

int get_subindex(NODE *node, VECTOR p);

void get_subpos(VECTOR mid, double size, int index, VECTOR pos);

void init_node(NODE *node, NODE *parent, VECTOR mid, double size, enum NODE_TYPE t, BODY *b);

void add_body(BODY *body, NODE *root, STACK *leaf_stack);

void free_tree(NODE *root);

void assign_cm(NODE *root, STACK *leaf_stack);

int accept(NODE *node, double theta, VECTOR pos);

void calculate_acceleration(BODY *body, NODE *root, double theta);

#endif
