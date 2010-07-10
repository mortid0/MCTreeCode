#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "stack.h"
#include "tree.h"
#include "treegrav.h"
#include "sl_list.h"


void direct_forces(int nbody, BODY *bodies)
{
	int i, j, k;
	double eps2;
	double dist3, dist;
	eps2 = 0.0;

	for (i = 0; i < nbody-1; i++)
	{
		for (j = i+1; j < nbody; j++)
		{
			dist = eps2;
			for (k = 0; k < 3; k++){dist3 = bodies[i].r[k]-bodies[j].r[k]; dist+=dist3*dist3;}
			dist = dist*sqrt(dist);
			for (k = 0; k < 3; k++)
			{
				bodies[i].a[k] += bodies[j].mass*(bodies[j].r[k] - bodies[i].r[k])/dist;
				bodies[j].a[k] += bodies[i].mass*(bodies[i].r[k] - bodies[j].r[k])/dist;
			}
		}
	}
}

void test_subindex()
{
	int i, s_i;
	VECTOR p;
	NODE root;
	root.mid[0] = 0.0;
	root.mid[1] = 0.0;
	root.mid[2] = 0.0;
	root.size = 1.0;
	for (i = 0; i < 8; i++)
	{
		get_subpos(root.mid, root.size, i, p);
		s_i = get_subindex(&root, p);
		printf("%i (%f;%f;%f) %i\n", i, p[0], p[1], p[2], s_i);
	}
}

int main(int argc, char *argv[])
{
	int nbody;
	double tstart = 0.0;
	nbody = get_nbody(argv[1]);
	SL_LIST *body_list = sl_list_create();
	in_sph(nbody, body_list, argv[1], &tstart);
//	in_barnes(nbody, body_list, argv[1]);
//	out_barnes(nbody, body_list, "first_out.dump");
	integrate(body_list, tstart, 3000.0, 5.0E-2, "dump/%05i.dump", atof(argv[2]));
	sl_list_free(body_list);
	return 0;
}

