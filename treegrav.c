#include "types.h"
#include "stack.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "params.h"
#include "io.h"
#include "sl_list.h"


void accurate_forces(SL_LIST *body_list)
{
	NODE *root;
	int i, k;
	double r;
	SL_LIST_ITEM *l_item;
	STACK *leaf_stack;
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	for (k = 0; k < 3; k++) { root->mid[k] = 0.0; }
	root->parent = NULL;
	root->type = T_NODE;
	for (i = 0; i < 8; i++) {root->kids[i] = NULL;}
	r = expandbox(body_list, root->mid);
	root->size = r;
	for (l_item = body_list->root; l_item != NULL; l_item = l_item->next)
	{
		add_body((BODY *)l_item->item, root, leaf_stack);
	}
	assign_cm(root, leaf_stack);
	for (l_item = body_list->root; l_item != NULL; l_item = l_item->next)
	{
		calculate_acceleration((BODY *)l_item->item, root, 1.0);
	}
	free_stack(leaf_stack);
	free_tree(root);
}

void friction_bodies(int nbody, BODY *bodies, double alfa, double dt)
{
	int i, k;
	double adt = alfa*dt;
	for (i = 0; i < nbody; i++)
	{
		for (k = 0; k < 3; k++)
		{
			bodies[i].v[k] -= bodies[i].v[k] * adt;
		}
	}
}

void set_accl_zero(SL_LIST *body_list)
{
	SL_LIST_ITEM *curr;
	BODY *body;
	for (curr = body_list->root; curr != NULL; curr = curr->next)
	{
		body = (BODY*)(curr->item);
		body->a[0] = 0.0; 
		body->a[1] = 0.0; 
		body->a[2] = 0.0;
	}

}

void integrate(SL_LIST *body_list, double tstart, double tstop, double tstep, char *filename_format, double alfafric)
{
	int k, step_num, nbody;
	double curr_step;
	char filename[20];
	SL_LIST_ITEM *curr;
	BODY *body;
	printf("tstep = %f;\n", tstep);
	step_num = 0;
	set_accl_zero(body_list);
	accurate_forces(body_list);

	for (curr_step = tstart; curr_step < tstop; curr_step += tstep)
	{
		nbody = 0;
		for (curr = body_list->root; curr != NULL; curr = curr->next)
		{
			body = ((BODY*)(curr->item));
			for (k = 0; k < 3; k++)
			{
				body->v[k] += 0.5 * tstep * body->a[k];
				body->r[k] += tstep * body->v[k];
			}
			nbody++;
		}
		set_accl_zero(body_list);
		accurate_forces(body_list);
		for (curr = body_list->root; curr != NULL; curr = curr->next)
		{
			body = ((BODY*)(curr->item));
			for (k = 0; k < 3; k++)
			{
				body->v[k] += 0.5 * tstep * body->a[k];
			}
		}
//		friction_bodies(nbody, bodies, alfafric, tstep*0.5);
		sprintf(filename, filename_format, step_num);
		printf("%i %s\n", step_num, filename);
		out_sph(nbody, body_list, filename, curr_step);
//		out_barnes(nbody, body_list, filename);
		step_num++;
	}
}
