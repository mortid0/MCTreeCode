#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "stack.h"
#include "tree.h"
#include "treegrav.h"


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
	BODY *bodies;
	int nbody;
	nbody = get_nbody(argv[1]);
	bodies = malloc(nbody*sizeof(BODY));
	in_barnes(nbody, bodies, argv[1]);
//	test_subindex();
	printf("read\n");
	integrate(nbody, bodies, 0.0, 10.0, 0.02, "dump/%04i.dump", atof(argv[2]));
//	test_subindex();
//	mc_forces(bodies);
//	accurate_forces(nbody, bodies);
//	direct_forces(bodies);
//	out_barnes(nbody, bodies, argv[2]);
	free(bodies);
	return 0;
}

