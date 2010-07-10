#ifndef TYPES_H
#define TYPES_H

#include "params.h"
#include "sl_list.h"

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
	int accepted;
	double h;  // diam of a particle
	double dens; // number of particles inside sphere 4/3 pi h^3
	SL_LIST_ITEM *l_item;
} BODY;

#define OCT_NUM 8

typedef struct _NODE{
	struct _NODE *parent;
	struct _NODE *kids[OCT_NUM];
	enum NODE_TYPE type;
	BODY *body;	// if T_LEAF==type, then b points to body in this leaf
	VECTOR mid; // center of node
	VECTOR cm;  // center mass position
	double mass;
	double size; // side length of cube
} NODE;

#define GAM 1.0
//(6.67E-11)

typedef struct {
	SL_LIST_ITEM *b1, *b2;
	double coll_time;
} BODY_PAIR;

#endif
