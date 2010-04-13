#include "types.h"
#include "params.h"
#include <math.h>
#include <stdlib.h>

// return size(length) of cube, with center c, that all particles from
// bodies[NB] fall inside.
double expandbox(BODY *bodies, int NB, VECTOR c)
{
	int i, k;
	double dmax, r;
	dmax = 0;
	r = 1.0;
	for (i = 0; i < NB; i++)
	{
		for (k = 0; k < NDIM; k++)
		{
			if (fabs(bodies[i].r[k]-c[k]) > dmax) {dmax = fabs(bodies[i].r[k]-c[k]);}
		}
	}
	while (r < 2.0*dmax)
	{
		r *=2.0;
	}
	return r;
}

// return number of cube where lies point p
// 7 - (0.75; 0.75; 0.75)
// 6 - (0.75; 0.75; 0.25)
// 5 - (0.75; 0.25; 0.75)
// ...
// 1 - (0.25; 0.25; 0.75)
// 0 - (0.25; 0.25; 0.25)
int get_subindex(NODE *node, VECTOR *p)
{
	int i;
	int index = 0;
	for (i = 0; i < 3; i++)
	{
		if (node->mid[2-i] < *p[2-i]) {index += (1<<i);} 
	}
	return index;
}

void get_subpos(NODE *node, int index, VECTOR *pos)
{
	double hsize = node->size * 0.5;
	*pos[0] = node->mid[0] - hsize * (0.5 - ((index & 4)>>2));
	*pos[1] = node->mid[1] - hsize * (0.5 - ((index & 2)>>1));
	*pos[2] = node->mid[2] - hsize * (0.5 - ((index & 1)));
}

// initialize node with parent, mid, size and type
void init_node(NODE *node, NODE *parent, VECTOR *mid, double size, enum NODE_TYPE t, BODY *b)
{
	node->parent = parent;
	node->mid[0] = *mid[0];
	node->mid[1] = *mid[1];
	node->mid[2] = *mid[2];
	node->size = size;
	node->type = t;
	node->body = b;
}

// Adds body to root node
void add_body(BODY *body, NODE *root)
{
	NODE *curr, *leaf;
	VECTOR pos;
	int curr_index, k;
	curr = root;
	curr_index = get_subindex(root, &body->r);
	while (T_NODE == curr->kids[curr_index]->type)
	{
		curr = curr->kids[curr_index];
		curr_index = get_subindex(curr, &body->r);
	}
	while (T_LEAF == curr->kids[curr_index]->type)
	{
		leaf = curr->kids[curr_index];
		curr->kids[curr_index] = malloc(sizeof(NODE));
		get_subpos(curr, curr_index, &pos);
		init_node(curr->kids[curr_index], curr, &pos, 0.5*curr->size, T_NODE, NULL);
		curr = curr->kids[curr_index];
		k = get_subindex(curr, &(leaf->body->r));
		curr->kids[k] = leaf;
		leaf->parent = curr;
		get_subpos(curr, k, &pos);
		leaf->mid[0] = pos[0]; leaf->mid[1] = pos[1]; leaf->mid[2] = pos[2];
		curr_index = get_subindex(curr, &body->r);
	}
	leaf = malloc(sizeof(NODE));
	get_subpos(curr, curr_index, &pos);
	init_node(leaf, curr, &pos, curr->size*0.5, T_LEAF, body);
	curr->kids[curr_index] = leaf;
}

