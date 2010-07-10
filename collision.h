#ifndef COLLISION_H
#define COLLISION_H

#include "types.h"

// return time when particles collide
// <=0 particles wouldn't collide
// >0 particles collide
double collision_time(VECTOR x1, VECTOR x2, VECTOR v1, VECTOR v2, double r1, double r2);

// return pointer to new body b3 = b1 + b2
// momentum conserve, not energy
BODY * make_cluster(BODY *b1, BODY *b2, double coll_time, double step_time);

#endif

