#include "types.h"
#include "stack.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "params.h"
#include "io.h"

#define NMC 16384

void mc_forces(int nbody, BODY *bodies)
{
	NODE *root;
	int i, j, k;
	STACK *leaf_stack;
	BODY *mc_bodies;
	mc_bodies = malloc(NMC*sizeof(BODY));
	i = 0;
	printf("Start mc_bodiess\n");
 	while (i < NMC)
	{
		do{
			j = rand() % nbody;
		}while (1 == bodies[j].accepted);
		bodies[j].accepted = 1;
		for (k = 0; k < 3; k++)
		{
			mc_bodies[i].r[k] = bodies[j].r[k];
			mc_bodies[i].v[k] = bodies[j].v[k];
			mc_bodies[i].a[k] = 0.0;
		}
		mc_bodies[i].mass = bodies[j].mass*(nbody/NMC);
		i++;
	}
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	for (k = 0; k < 3; k++) { root->mid[k] = 0.0; }
	root->parent = NULL;
	root->type = T_NODE;
	root->size = expandbox(bodies, nbody, root->mid);
	printf("fit size\n");
	for (i = 0; i < NMC/*  NBODY*/; i++)
	{
		add_body(&(mc_bodies[i]), root, leaf_stack);
	}
	assign_cm(root, leaf_stack);
	for (i = 0; i < nbody; i++)
	{
		calculate_acceleration(&bodies[i], root, 1.0);
	}
	free_stack(leaf_stack);
	free_tree(root);
	free(mc_bodies);
	out_barnes(nbody, bodies,"out.131072_mc_full");
}

void accurate_forces(int nbody, BODY *bodies)
{
	NODE *root;
	int i, k;
	double r;
	STACK *leaf_stack;
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	for (k = 0; k < 3; k++) { root->mid[k] = 0.0; }
	root->parent = NULL;
	root->type = T_NODE;
	for (i = 0; i < 8; i++) {root->kids[i] = NULL;}
	r = expandbox(bodies, nbody, root->mid);
	root->size = r;
	for (i = 0; i < nbody; i++)
	{
		add_body(&(bodies[i]), root, leaf_stack);
	}
	assign_cm(root, leaf_stack);
	for (i = 0; i < nbody; i++)
	{
		calculate_acceleration(&bodies[i], root, 0.7);
	}
	free_stack(leaf_stack);
	free_tree(root);
}

void integrate(int nbody, BODY *bodies, double tstart, double tstop, int steps, char *filename_format)
{
	int i, k, step_num;
	double tstep, curr_step;
	char filename[20];
	tstep = (tstop-tstart)/steps;
	printf("tstep = %f;\n", tstep);
	step_num = 0;
	for (i = 0; i < nbody; i++){bodies[i].a[0] = 0.0; bodies[i].a[1] = 0.0; bodies[i].a[2] = 0.0;}
	accurate_forces(nbody, bodies);

	for (curr_step = tstart; curr_step < tstop; curr_step += tstep)
	{
		for (i = 0; i < nbody; i++)
		{
			for (k = 0; k < 3; k++)
			{
				bodies[i].v[k] += 0.5 * tstep * bodies[i].a[k];
				bodies[i].r[k] += tstep * bodies[i].v[k];
			}
		}
		for (i = 0; i < nbody; i++){bodies[i].a[0] = 0.0; bodies[i].a[1] = 0.0; bodies[i].a[2] = 0.0;}
		accurate_forces(nbody, bodies);
		for (i = 0; i < nbody; i++)
		{
			for (k = 0; k < 3; k++)
			{
				bodies[i].v[k] += 0.5 * tstep * bodies[i].a[k];
			}
		}
		sprintf(filename, filename_format, step_num);
		printf("%i %s\n", step_num, filename);
		out_barnes(nbody, bodies, filename);
		step_num++;
	}
}
