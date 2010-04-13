#ifndef TYPES_H
#define TYPES_H

#include "params.h"

enum NODE_TYPE{
	T_NODE,
	T_LEAF
};

typedef double VECTOR[NDIM];

typedef struct{
	double mass;
	VECTOR v; // velocity
	VECTOR a; // acceleration
	VECTOR r; // position
} BODY;

#define OCT_NUM 8

typedef struct _NODE{
	struct _NODE *parent;
	struct _NODE *kids[OCT_NUM];
	enum NODE_TYPE type;
	BODY *b;	// if T_LEAF==type, then b points to body in this leaf
	VECTOR mid; // center of node
	VECTOR cm;  // center mass position
	double mass;
} NODE;



#endif
