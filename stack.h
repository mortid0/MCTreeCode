#ifndef STACK_H
#define STACK_H

typedef struct _STACK_ITEM{
	void *item;
	struct _STACK_ITEM *next;
} STACK_ITEM;

typedef struct _STACK{
	STACK_ITEM *root;
	int size;
} STACK;


int push(STACK *stack, void *el);

STACK* create_stack();

void* pop(STACK *stack);

int free_stack(STACK *stack);

#endif
