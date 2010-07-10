#ifndef TREEGRAV_H
#define TREEGRAV_H

#include "types.h"
#include "sl_list.h"
#include "stack.h"

void accurate_forces(SL_LIST *body_list, STACK *collision_pair, STACK *collision_bodies, double time_step);

void integrate(SL_LIST *body_list, double tstart, double tstop, double tstep, char *filename_format, double alfafric);
#endif
