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
		fprintf(f, " %f %f %f\n", bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
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
		printf("%f\n", bodies[i].mass);
		fscanf(f," %lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
		fscanf(f," %lf %lf %lf\n", &(bodies[i].v[0]),&(bodies[i].v[1]),&(bodies[i].v[2]));
		i++;
	}
	fclose(f);
}
