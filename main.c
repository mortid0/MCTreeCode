#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "stack.h"
#include "tree.h"

void accurate_forces(BODY *bodies)
{
	int i, j;
	VECTOR acc[NBODY];
	double max_error, max_eps_error, avg_error, disp_error, tmp_error;
	double a_mod;
	int num_max_eps;
	max_error = 0.0;
	max_eps_error = 0.0;
	avg_error = 0.0;
	disp_error = 0.0;
	for (i = 0; i < NBODY; i++)
	{
		acc[i][0] = 0.0;
		acc[i][1] = 0.0;
		acc[i][2] = 0.0;
		for (j = 0; j < NBODY; j++)
		{
			if ( i==j ) {continue;}
			double dist = sqrt(pow(bodies[i].r[0]-bodies[j].r[0],2) + pow(bodies[i].r[1]-bodies[j].r[1],2) + pow(bodies[i].r[2]-bodies[j].r[2],2));
			double dist3 = dist*dist*dist;
			acc[i][0] += (bodies[j].r[0] - bodies[i].r[0]) * (bodies[j].mass/dist3);
			acc[i][1] += (bodies[j].r[1] - bodies[i].r[1]) * (bodies[j].mass/dist3);
			acc[i][2] += (bodies[j].r[2] - bodies[i].r[2]) * (bodies[j].mass/dist3);
		}
//		printf("acc (%f;%f;%f)\n", acc[i][0], acc[i][1],acc[i][2]);
//		printf("tree(%f;%f;%f)\n", bodies[i].a[0], bodies[i].a[1], bodies[i].a[2]);
		tmp_error = 0.0;
		for (j = 0; j < 3; j++)
		{
			tmp_error += pow(bodies[i].a[j] - acc[i][j],2);
		}
		tmp_error = sqrt(tmp_error);
		disp_error += tmp_error*tmp_error;
		avg_error += tmp_error;
		if (tmp_error > max_error) {max_error = tmp_error;}
		a_mod = sqrt(pow(bodies[i].a[2],2) + pow(bodies[i].a[1],2) + pow(bodies[i].a[2],2));
		if (tmp_error/a_mod > max_eps_error) {max_eps_error = tmp_error/a_mod;num_max_eps = i;}
	}
	i = num_max_eps;
	avg_error /= NBODY;
	disp_error /=NBODY;
	disp_error = sqrt(disp_error - avg_error*avg_error);
	printf("Errors report:\n");
	printf("Avg = %f;\n", avg_error);
	printf("Max = %f;\n", max_error);
	printf("E_m = %f%;\n", max_eps_error*100);
	printf("BH = (%f; %f; %f) Accurate (%f;%f;%f),  num = %i\n", bodies[i].a[0],bodies[i].a[1], bodies[i].a[2], acc[i][0], acc[i][1], acc[i][2], i);
	printf("Dis = %f;\n", disp_error);
}

void compare_to_barnes(BODY *bodies)
{
	int i, j;
	VECTOR acc[NBODY];
	double max_error, max_eps_error, avg_error, disp_error, tmp_error;
	double a_mod;
	int num_max_eps;
	max_error = 0.0;
	max_eps_error = 0.0;
	avg_error = 0.0;
	disp_error = 0.0;
	for (i = 0; i < NBODY; i++)
	{
		acc[i][0] = bodies[i].a[0];
		acc[i][1] = bodies[i].a[1];
		acc[i][2] = bodies[i].a[2];
		bodies[i].a[0] = 0.0;
		bodies[i].a[1] = 0.0;
		bodies[i].a[2] = 0.0;
	}
	NODE *root;
	STACK *leaf_stack;
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	root->mid[0], root->mid[1], root->mid[2] = 0.0, 0.0, 0.0;
	root->parent = NULL;
	root->type = T_NODE;
	root->size = expandbox(bodies, 16384, root->mid);
	printf("fit size\n");
	for (i = 0; i < NBODY; i++)
	{
		add_body(&bodies[i], root, leaf_stack);
	}
	printf("added bodies\n");
	assign_cm(root, leaf_stack);
	printf("assigned cm\n");
	for (i = 0; i < NBODY; i++)
	{
		calculate_acceleration(&bodies[i], root, 1.0);
	}
	printf("calculated acc\n");
	free_stack(leaf_stack);
	free_tree(root);
	for (i = 0; i < NBODY; i++)
	{
		tmp_error = 0.0;
		for (j = 0; j < 3; j++)
		{
			tmp_error += pow(bodies[i].a[j] - acc[i][j],2);
		}
		tmp_error = sqrt(tmp_error);
		disp_error += tmp_error*tmp_error;
		avg_error += tmp_error;
		if (tmp_error > max_error) {max_error = tmp_error;}
		a_mod = sqrt(pow(bodies[i].a[2],2) + pow(bodies[i].a[1],2) + pow(bodies[i].a[2],2));
		if (tmp_error/a_mod > max_eps_error) {max_eps_error = tmp_error/a_mod;num_max_eps = i;}
	}
	i = num_max_eps;
	avg_error /= NBODY;
	disp_error /=NBODY;
	disp_error = sqrt(disp_error - avg_error*avg_error);
	printf("Errors report:\n");
	printf("Avg = %f;\n", avg_error);
	printf("Max = %f;\n", max_error);
	printf("E_m = %f%;\n", max_eps_error*100);
	printf("MC = (%f; %f; %f) BH (%f;%f;%f),  num = %i\n", bodies[i].a[0],bodies[i].a[1], bodies[i].a[2], acc[i][0], acc[i][1], acc[i][2], i);
	printf("Dis = %f;\n", disp_error);
}

#define NMC 16384
void mc_forces(BODY *bodies)
{
	NODE *root;
	int i, j;
	STACK *leaf_stack;
	BODY *mc_bodies;
	mc_bodies = malloc(NMC*sizeof(BODY));
	i = 0;
	printf("Start mc_bodiess\n");
	while (i < NMC)
	{
		do{
			j = rand() % NBODY;
		}while (1 == bodies[j].accepted);
		bodies[j].accepted = 1;
		mc_bodies[i].r[0] = bodies[j].r[0];
		mc_bodies[i].r[1] = bodies[j].r[1];
		mc_bodies[i].r[2] = bodies[j].r[2];
		mc_bodies[i].v[0] = bodies[j].v[0];
		mc_bodies[i].v[1] = bodies[j].v[1];
		mc_bodies[i].v[2] = bodies[j].v[2];
		mc_bodies[i].mass = bodies[j].mass*(NBODY/NMC);
		mc_bodies[i].a[0] = 0.0;
		mc_bodies[i].a[1] = 0.0;
		mc_bodies[i].a[2] = 0.0;
		i++;
	}
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	root->mid[0] = 0.0;
	root->mid[1] = 0.0;
	root->mid[2] = 0.0;
	root->parent = NULL;
	root->type = T_NODE;
	root->size = expandbox(mc_bodies, NMC, root->mid);
	printf("fit size\n");
	for (i = 0; i < NMC; i++)
	{
//		printf("add_body %i; (%4.4f;%4.4f;%4.4f)\n", i, mc_bodies[i].r[0], mc_bodies[i].r[1], mc_bodies[i].r[2]);
		add_body(&(mc_bodies[i]), root, leaf_stack);
	}
	printf("added bodies\n");
	assign_cm(root, leaf_stack);
	printf("assigned cm\n");
	for (i = 0; i < NBODY; i++)
	{
		calculate_acceleration(&bodies[i], root, 1.0);
	}
	printf("calculated acc\n");
	free_stack(leaf_stack);
	free_tree(root);
	free(mc_bodies);
	compare_to_barnes(bodies);
}

void calculate_forces(BODY *bodies)
{
	NODE *root;
	int i;
	STACK *leaf_stack;
	leaf_stack = create_stack();
	root = malloc(sizeof(NODE));
	root->mid[0], root->mid[1], root->mid[2] = 0.0, 0.0, 0.0;
	root->parent = NULL;
	root->type = T_NODE;
	root->size = expandbox(bodies, 16384, root->mid);
	printf("fit size\n");
	for (i = 0; i < NBODY; i++)
	{
		add_body(&bodies[i], root, leaf_stack);
	}
	printf("added bodies\n");
	assign_cm(root, leaf_stack);
	printf("assigned cm\n");
	for (i = 0; i < NBODY; i++)
	{
		calculate_acceleration(&bodies[i], root, 1.0);
	}
	printf("calculated acc\n");
	free_stack(leaf_stack);
	free_tree(root);
	accurate_forces(bodies);
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
	bodies = malloc(131072*sizeof(BODY));
	in_barnes(bodies, "test.out131072");
	printf("read\n");
	mc_forces(bodies);
//	calculate_forces(bodies);
	out(bodies, "out");
	free(bodies);
//	test_subindex();
	return 0;
}

