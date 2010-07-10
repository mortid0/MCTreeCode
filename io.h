#ifndef IO_H
#define IO_H

#include "types.h"
#include "sl_list.h"

int out(int nbody, BODY *bodies, char *filename, double curr_time);

int in(int nbody, BODY *bodies, char *filename);

void in_barnes(int nbody, SL_LIST *body_list, char *filename);

void out_barnes(int nbody, SL_LIST *body_list, char *filename);

int get_nbody(char *file_name);

void out_sph(int nbody, SL_LIST *body_list, char *filename, double curr_time);

void in_sph(int nbody, SL_LIST *body_list, char *filename, double *curr_time);

#endif
