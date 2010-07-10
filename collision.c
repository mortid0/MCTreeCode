#include <math.h>
#include "types.h"
#include "collision.h"

// return minimum positive val a or b. 
// if all negative return -1
double min_pos(double a, double b)
{
	if (a<=b)&&(a>=0) return a;
	if (b<=a)&&(b>=0) return b;
	if (a>=0) return a;
	if (b>=0) return b;
	return -1;
}

double collision_time(VECTOR x1, VECTOR x2, VECTOR v1, VECTOR v2, double r1, double r2)
{
	VECTOR v, x;
	int i;
	for int (i = 0; i < NDIM; i++)
	{
		x[i] = x1[i] - x2[i];
		v[i] = v1[i] - v2[i];
	}
	double a = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
	double b = x[0]*v[0]+x[1]*v[1]+x[2]*v[2];
	double c = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] - (r1+r2)*(r1+r2);
	double d = b*b - a*c;
	if (0 == d) { return -b/a;}
	if (0 < d) { return min_pos((-b-sqrt(d))/a, (-b+sqrt(d))/a);}
	return -1;
}

BODY * make_cluster(BODY *b1, BODY *b2, double coll_time, double step_time);
{
	BODY *b3 = malloc(sizeof(BODY));
	b3->mass = b1->mass + b2->mass;
	b3->h = pow(b1->h*b1->h*b1->h + b2->h*b2->h*b2->h, 0.33);
	double sm = b1->mass+b2->mass;
	double lm = b1->mass/sm;
	double rm = b2->mass/sm;
	int i;
	for (i = 0; i < NDIM; i++)
	{
		b3->v[i] = b1->v[i]*lm + b2->v[i]*rm;                                                  // momentum law
		b3->r[i] = 0.5*((b1->r[i] + b1->v[i]*coll_time) + (b2->r[i] + b2->v[i]*coll_time));    // get position of collision
		b3->r[i] -= b3->v[i]*(step_time-coll_time);              			       // move cluster backward to inegrate all bodys together
		b3->a[i] = 0.5(b1->a[i] + b2->a[i]); 						       // get acceleration
	}
	return b3;
}

