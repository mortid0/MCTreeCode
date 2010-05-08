#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "params.h"
#include <gsl/gsl_histogram.h>
#include <math.h>

#define DFMT " %14.7E"


void out(int nbody, BODY *bodies, char *filename, double curr_time)
{
	int i;
	FILE *f;
	f = fopen(filename, "w+");
	fprintf(f, "#%i\n", nbody);
	fprintf(f, "#%i\n", NDIM);
	fprintf(f, "#%f\n", curr_time);
	for (i = 0; i < nbody; i++)
	{
		fprintf(f, DFMT DFMT DFMT DFMT, bodies[i].mass, bodies[i].r[0], bodies[i].r[1], bodies[i].r[2]);
		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
//		fprintf(f, DFMT DFMT DFMT "\n", bodies[i].a[0], bodies[i].a[1], bodies[i].a[2]);
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

void in_sph(int nbody, BODY *bodies, char *filename, double *curr_time)
{
	FILE *f;
	int test_nb, test_nd, i;
	f = fopen(filename, "r");
	if (NULL == f) {printf("File %s not found\n", filename); exit(-1);}
	fscanf(f, "%i\n", &test_nb);
	fscanf(f, "%i\n", &test_nd);
	fscanf(f, "%lf\n", curr_time);
	for (i = 0; i < nbody; i++)
	{
		fscanf(f,"%lf %lf %lf", &(bodies[i].r[0]),&(bodies[i].r[1]),&(bodies[i].r[2]));
		fscanf(f," %lf %lf %lf", &(bodies[i].v[0]),&(bodies[i].v[1]),&(bodies[i].v[2]));
		fscanf(f," %lf %lf %lf\n", &(bodies[i].mass), &(bodies[i].dens),&(bodies[i].h));
	}
	fclose(f);
}

#define BIN_NUM 40
// Assumed, that sun has last number in bodies;
void out_sph(int nbody, BODY *bodies, char *filename, double curr_time)
{
	int i;
	FILE *f;
	double v_curr, r_curr, v_kepler, m_sun;
	gsl_histogram *v_distrib;
	v_distrib = gsl_histogram_alloc(BIN_NUM);
	gsl_histogram_set_ranges_uniform(v_distrib, 0.0, 2.0);
	m_sun = bodies[nbody-1].mass;
	f = fopen(filename, "w+");
	fprintf(f, "#%i\n", nbody);
	fprintf(f, "#%i\n", NDIM);
	fprintf(f, "#%f\n", curr_time);
	v_kepler = 0.0;
	for (i = 0; i < nbody; i++)
	{
		v_curr = sqrt(bodies[i].v[0]*bodies[i].v[0] + bodies[i].v[1]*bodies[i].v[1] + bodies[i].v[2]*bodies[i].v[2]); 
		r_curr = sqrt(bodies[i].r[0]*bodies[i].r[0] + bodies[i].r[1]*bodies[i].r[1] + bodies[i].r[2]*bodies[i].r[2]); 
		if (r_curr > 0.0001) {v_kepler = sqrt(m_sun/r_curr);}
		gsl_histogram_increment(v_distrib, v_curr/v_kepler);
		fprintf(f, DFMT DFMT DFMT, bodies[i].r[0], bodies[i].r[1], bodies[i].r[2]);
		fprintf(f, DFMT DFMT DFMT, bodies[i].v[0], bodies[i].v[1], bodies[i].v[2]);
		fprintf(f, " %i %i %i\n", (int)(bodies[i].mass/bodies[0].mass), (int)(bodies[i].dens), (int)(bodies[i].h));
	}
	fclose(f);
	FILE *f_distrib;
	char distrib_name[30];
	sprintf(distrib_name, "%s.distrib", filename);
	f_distrib = fopen(distrib_name, "w+");
	fprintf(f_distrib, "# Mean  = %7.7E\n", gsl_histogram_mean(v_distrib));
	fprintf(f_distrib, "# Sigma = %7.7E\n", gsl_histogram_sigma(v_distrib));
	gsl_histogram_fprintf(f_distrib, v_distrib, "%7.7E", "%7.7E");
	gsl_histogram_free(v_distrib);
	fclose(f_distrib);
}

