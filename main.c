#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "stack.h"

int main(int argc, char *argv[])
{
	BODY *bodies;
	bodies = malloc(2*sizeof(BODY));
	in(bodies, "in");
	out(bodies, "out");
	free(bodies);
	return 0;
}

