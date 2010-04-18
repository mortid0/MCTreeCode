#ifndef IO_H
#define IO_H

#include "types.h"

int out(BODY *bodies, char *filename);

int in(BODY *bodies, char *filename);

void in_barnes(BODY *bodies, char *filename);

#endif
