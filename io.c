#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "params.h"

#define DFMT " %14.7E"

void out(int nbody, BODY *bodies, char *filename)
{
	int i;
	FILE *f;
	f = fopen(filename, "w+");
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT DFMT DFMT DFMT, bodies[i].mass, bodies[i].r[0], bodies[i].r[1], bodies[i].r[2]);
		fprintf(f, DFMT DFMT DFMT, bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].a[0], bodies[i].a[1], bodies[i].a[2]);
	}
	fclose(f);
}

void in(int nbody, BODY *bodies, char *filename)
{
	int i, tst_nbody;
	FILE *f;
	f = fopen(filename, "r");
	if (NULL == f) {printf("File '%s' not found\n", filename); exit(-1);}
	printf("Starting input\n");
	fscanf(f, "%i", &tst_nbody);
	if (nbody != tst_nbody) {printf("Nbody error for file %s; have = %i; in file %i\n", filename, nbody, tst_nbody); exit(-1);}
	for (i = 0; i < nbody; i++)
	{
		fscanf(f, "%lf", &(bodies[i].mass));
		fscanf(f," %lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
		fscanf(f," %lf %lf %lf\n", &(bodies[i].v[0]),&(bodies[i].v[1]),&(bodies[i].v[2]));
		bodies[i].a[0] = 0.0;
		bodies[i].a[1] = 0.0;
		bodies[i].a[2] = 0.0;
	}
	fclose(f);
}

void in_barnes(int nbody, BODY *bodies, char *filename)
{
	int i, tst_nbody;
	FILE *f;
	double t_start;
	f = fopen(filename, "r");
	if (NULL == f) {printf("File '%s' not found\n", filename); exit(-1);}
	fscanf(f, "%i", &tst_nbody);
	if (nbody != tst_nbody) {printf("Nbody error for file %s; have = %i; in file %i\n", filename, nbody, tst_nbody); exit(-1);}
	fscanf(f, "%i\n", &i);// dim
	fscanf(f, "%lf\n", &t_start);// tstart
	for (i = 0; i < nbody; i++)
	{
		fscanf(f,"%lf\n", (&bodies[i].mass));
	}
	for (i = 0; i < nbody; i++)
	{
		fscanf(f," %lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
	}
	for (i = 0; i < nbody; i++)
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

void out_barnes(int nbody, BODY *bodies, char *filename)
{
	int i;
	FILE *f;
	f = fopen(filename, "w+");
	printf("start output\n");
	fprintf(f, "%i\n", nbody);
	if (NULL == f) {printf("File '%s' not found\n", filename); exit(-1);}
	fprintf(f, "3\n");// dim
	fprintf(f, "0.0\n");// tstart
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT "\n", bodies[i].mass);
	}
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].r[0], bodies[i].r[1], bodies[i].r[2]);
	}
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
	}
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].a[0], bodies[i].a[1], bodies[i].a[2]);
	}
	fclose(f);
}

int get_nbody(char *file_name)
{
	FILE *f;
	int nbody;
	f = fopen(file_name, "r");
	if (NULL == f) {printf("File %s not found\n", file_name); exit(-1);}
	fscanf(f, "%i", &nbody);
	fclose(f);
	return nbody;
}
