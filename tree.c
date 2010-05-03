#include "types.h"
#include "params.h"
#include "stack.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
	//printf("r = %f;\n", r);
	return r;
}

// return number of cube where lies point p
// 7 - (0.75; 0.75; 0.75)
// 6 - (0.75; 0.75; 0.25)
// 5 - (0.75; 0.25; 0.75)
// ...
// 1 - (0.25; 0.25; 0.75)
// 0 - (0.25; 0.25; 0.25)
int get_subindex(NODE *node, VECTOR p)
{
	int i;
	int index = 0;
	for (i = 0; i < 3; i++)
	{
		if (node->mid[i] < p[i]) {index += (1<<(2-i));} 
	}
	return index;
}

void get_subpos(VECTOR mid, double size, int index, VECTOR pos)
{
	double hsize = size * 0.5;
	pos[0] = mid[0] - hsize * (0.5 - ((index & 4)>>2));
	pos[1] = mid[1] - hsize * (0.5 - ((index & 2)>>1));
	pos[2] = mid[2] - hsize * (0.5 - ((index & 1)));
}

// initialize node with parent, mid, size and type
void init_node(NODE *node, NODE *parent, VECTOR mid, double size, enum NODE_TYPE t, BODY *b)
{
	node->parent = parent;
	node->mid[0] = mid[0];
	node->mid[1] = mid[1];
	node->mid[2] = mid[2];
	node->size = size;
	node->type = t;
	node->body = b;
	node->cm[0] = 0.0;
	node->cm[1] = 0.0;
	node->cm[2] = 0.0;
	node->mass = 0.0;
	int i;
	for (i = 0; i < 8; i++) {node->kids[i] = NULL;}
}

// Adds body to root node
// and populates leaf_stack with leaves for easy cm calculation
void add_body(BODY *body, NODE *root, STACK *leaf_stack)
{
	NODE *curr, *leaf;
	VECTOR pos;
	int curr_index, k;
	curr = root;
	curr_index = get_subindex(root, body->r);
//	printf("body (%f;%f;%f)\n", body->r[0], body->r[1], body->r[2]);
//	printf("root (%f;%f;%f) %i\n", curr->mid[0], curr->mid[1], curr->mid[2], curr_index);
//	printf("%p\n",curr->kids[curr_index]);
	while ((NULL != curr->kids[curr_index]) && (T_NODE == curr->kids[curr_index]->type))   // first traverse tree while body fall in node
	{
//		printf("node (%f;%f;%f) %i\n", curr->mid[0], curr->mid[1], curr->mid[2], curr_index);
		curr = curr->kids[curr_index];
		curr_index = get_subindex(curr, body->r);
	}
	
	if (NULL != curr->kids[curr_index]) 
	{
		leaf = curr->kids[curr_index];
//		printf("leaf (%f;%f;%f)\n", leaf->body->r[0], leaf->body->r[1], leaf->body->r[2]);
	}

	while ((NULL != curr->kids[curr_index]) && (T_LEAF == curr->kids[curr_index]->type)) // then go throw leaves
	{
		leaf = curr->kids[curr_index];
		curr->kids[curr_index] = malloc(sizeof(NODE));
		get_subpos(curr->mid, curr->size, curr_index, pos);
		init_node(curr->kids[curr_index], curr, pos, 0.5*curr->size, T_NODE, NULL); // Make new node and put there new leaves
		curr = curr->kids[curr_index];
		k = get_subindex(curr, leaf->body->r);
		curr->kids[k] = leaf;
		leaf->parent = curr;
		get_subpos(curr->mid, curr->size, k, pos);
		leaf->mid[0] = pos[0]; leaf->mid[1] = pos[1]; leaf->mid[2] = pos[2];
		curr_index = get_subindex(curr, body->r);
	}
	leaf = malloc(sizeof(NODE));
	if (NULL == leaf) {printf("Can't allocate mem\n");}
	push(leaf_stack, leaf);
	get_subpos(curr->mid, curr->size, curr_index, pos);
	init_node(leaf, curr, pos, curr->size*0.5, T_LEAF, body);
	curr->kids[curr_index] = leaf;
}

void free_tree(NODE *root)
{
	STACK *stack;
	int i;
	NODE *node;
	stack = create_stack();
	push(stack, root);
	while (stack->size > 0)
	{
		node = (NODE *)pop(stack);
		if (T_LEAF == node->type)		// element is leaf, so free it and go to next element
		{
			free(node);
			continue;
		}
		for (i = 0; i < 8; i++)			// element is node, push all it's kids to stack
		{
			if (NULL != node->kids[i])
			{
				push(stack, node->kids[i]);
			}
		}
	}
	free_stack(stack);
}

// assign center of mass of nodes, walking from down to up
// uses leaf_stack to get leaves
void assign_cm(NODE *root, STACK *leaf_stack)
{
	NODE *leaf, *node;
	BODY *body;
	STACK *node_stack;
	int i;
//	printf("assign cm %i\n", leaf_stack->size);
	while (leaf_stack->size > 0)
	{
		leaf = (NODE *)pop(leaf_stack);
		body = leaf->body;
		leaf->cm[0] = body->r[0];
		leaf->cm[1] = body->r[1];
		leaf->cm[2] = body->r[2];
		leaf->mass = body->mass;
		node = leaf;
		while (NULL != node->parent)				// only root node has NULL parent
		{
			node = node->parent;
			node->cm[0] += body->r[0] * body->mass;
			node->cm[1] += body->r[1] * body->mass;
			node->cm[2] += body->r[2] * body->mass;
			node->mass += body->mass;
		}
//		printf("leaf m=%f cm(%f;%f;%f) r(%f;%f;%f)\n", leaf->mass, leaf->cm[0], leaf->cm[1], leaf->cm[2], leaf->mid[0], leaf->mid[1], leaf->mid[2]);
//		printf("body m=%f rd(%f;%f;%f)\n", leaf->body->mass, leaf->body->r[0], leaf->body->r[1], leaf->body->r[2]);

	}
	node_stack = create_stack();
	push(node_stack, root);
	while(node_stack->size > 0)
	{
		node = (NODE *)pop(node_stack);
		if (T_LEAF == node->type) {continue;}
		for (i = 0; i < 8; i++)
		{
			if (NULL != node->kids[i]) {push(node_stack, node->kids[i]);}
		}
		node->cm[0] /= node->mass;
		node->cm[1] /= node->mass;
		node->cm[2] /= node->mass;
	}
	free_stack(node_stack);
}

int accept(NODE *node, double theta, VECTOR pos)
{
   	double dist = sqrt(pow(pos[0]-node->cm[0],2) + pow(pos[1] - node->cm[1],2) + pow(pos[2]-node->cm[2],2));

	if (node->size/dist < theta) {return 1;}
	return 0;
/*  	double dmax = node->size;
	double dsq = 0.0;
	int i, res;
	double dk, rcrit2;
	for (i = 0; i < 3; i++)
	{
		dk = fabs(node->mid[i] - pos[i]);
		if (dk > node->size) {dmax = dk;}
		dk -= node->size;
		if (dk > 0) {dsq += dk*dk; }
	}
	rcrit2 = pow(node->size/theta + sqrt(pow(node->cm[0]-node->mid[0], 2) + pow(node->cm[1]-node->mid[1], 2) + pow(node->cm[2]-node->mid[2], 2)),2);
	res = ((dsq > rcrit2) && (dmax>1.5*node->size))?1:0;
//	printf("node (%f; %f; %f;) body (%f; %f; %f) res=%i\n", node->mid[0], node->mid[1], node->mid[2], *pos[0], *pos[1], *pos[2], res);
	return res;*/
}

void calculate_acceleration(BODY *body, NODE *root, double theta)
{
	NODE *node, *leaf;
	STACK *stack;
	int i;
	double eps2 = 0.005*0.005;
	body->a[0] = 0.0;
	body->a[1] = 0.0;
	body->a[2] = 0.0;
	stack = create_stack();
	push(stack, root);
	while (stack->size> 0)
	{
		node = (NODE *)pop(stack);
		leaf = node;
		if ((T_LEAF == node->type) && (node->body == body)) {continue;} // don't act on self

		if ((T_LEAF == node->type) || (accept(node, theta, body->r)))
//		if (accept(node, theta, body->r))
		{
			double dist = sqrt(pow(body->r[0]-node->cm[0],2) + pow(body->r[1] - node->cm[1],2) + pow(body->r[2]-node->cm[2],2) + eps2);
			double dist3 = dist*dist*dist;
			body->a[0] += (node->cm[0] - body->r[0]) * (node->mass/dist3);
			body->a[1] += (node->cm[1] - body->r[1]) * (node->mass/dist3);
			body->a[2] += (node->cm[2] - body->r[2]) * (node->mass/dist3);
		}
		else
		{
			for (i = 0; i < 8; i++)
			{
				if (NULL != node->kids[i]) {push(stack, node->kids[i]);}
			}
		}
	}
	free_stack(stack);
}

