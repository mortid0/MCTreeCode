#ifndef TREEGRAV_H
#define TREEGRAV_H

#include "types.h"

void accurate_forces(int nbody, BODY *bodies);

void mc_forces(int nbody, BODY *bodies);

void integrate(int nbody, BODY *bodies, double tstart, double tstop, double tstep, char *filename_format);
#endif
