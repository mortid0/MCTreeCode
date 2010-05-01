#ifndef IO_H
#define IO_H

#include "types.h"

int out(int nbody, BODY *bodies, char *filename, double curr_time);

int in(int nbody, BODY *bodies, char *filename);

void in_barnes(int nbody, BODY *bodies, char *filename);

void out_barnes(int nbody, BODY *bodies, char *filename);

int get_nbody(char *file_name);

#endif
