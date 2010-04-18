#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "params.h"

void out(BODY *bodies, char *filename)
{
	int i;
	FILE *f;
	f = fopen(filename, "w+");
	fprintf(f, "%i\n", NBODY);
	for (i = 0; i < NBODY; i++)
	{
		fprintf(f, "%f %f %f %f", bodies[i].mass, bodies[i].r[0], bodies[i].r[1], bodies[i].r[2]);
		fprintf(f, " %f %f %f", bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
		fprintf(f, " %f %f %f\n", bodies[i].a[0], bodies[i].a[1], bodies[i].a[2]);
	}
	fclose(f);
}

void in(BODY *bodies, char *filename)
{
	int i;
	FILE *f;
	f = fopen(filename, "r");
	if (NULL == f) {printf("File '%s' not found\n", filename); exit(-1);}
	printf("Starting input\n");
	fscanf(f, "%i", &NBODY);
	printf("NBODY = %i\n", NBODY);
	i = 0;
	while(!feof(f))
	{
		fscanf(f, "%lf", &(bodies[i].mass));
		fscanf(f," %lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
		fscanf(f," %lf %lf %lf\n", &(bodies[i].v[0]),&(bodies[i].v[1]),&(bodies[i].v[2]));
		bodies[i].a[0] = 0.0;
		bodies[i].a[1] = 0.0;
		bodies[i].a[2] = 0.0;
		i++;
	}
	fclose(f);
}

void in_barnes(BODY *bodies, char *filename)
{
	int i;
	FILE *f;
	double t_start;
	f = fopen(filename, "r");
	printf("start input\n");
	fscanf(f, "%i", &NBODY);
	if (NULL == f) {printf("File '%s' not found\n", filename); exit(-1);}
	printf("NBODY = %i;\n", NBODY);
	fscanf(f, "%i\n", &i);// dim
	fscanf(f, "%lf\n", &t_start);// tstart
	for (i = 0; i < NBODY; i++)
	{
		fscanf(f,"%lf\n", (&bodies[i].mass));
	}
	for (i = 0; i < NBODY; i++)
	{
		fscanf(f," %lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
	}
	for (i = 0; i < NBODY; i++)
	{
		fscanf(f," %lf %lf %lf", &(bodies[i].v[0]),&(bodies[i].v[1]),&(bodies[i].v[2]));
		bodies[i].a[0] = 0.0;
		bodies[i].a[1] = 0.0;
		bodies[i].a[2] = 0.0;
		bodies[i].accepted = 0;
	}
	fclose(f);
	printf("Finish read\n");
}
