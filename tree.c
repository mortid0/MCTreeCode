#include "types.h"
#include "params.h"

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


