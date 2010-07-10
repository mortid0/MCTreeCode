#include "stack.h"
#include <stdlib.h>

STACK* create_stack()
{
	STACK* l = malloc(sizeof(STACK));
	l->size = 0;
	l->root = NULL;
	return l;
}

int push(STACK *stack,void *el)
{
	STACK_ITEM *head;
	head = malloc(sizeof(STACK_ITEM));
	head->item = el;
	head->next = stack->root;
	stack->root = head;
	stack->size++;
	return 0;
}

void* pop(STACK *stack)
{
	if (0 == stack->size) {return NULL;}
	void *item = stack->root->item;
	STACK_ITEM *i = stack->root;
	stack->root = i->next;
	free(i);
	stack->size--;
	return item;
}

int free_stack(STACK *stack)
{
	while(NULL != stack->root)
	{
		STACK_ITEM *i = stack->root;
		stack->root = i->next;
		free(i);
	}
	free(stack);
	return 0;
}

int stack_find(STACK *stack, void *el)
{
	STACK_ITEM *i = stack->root;
	while (NULL != i)
	{
		if (i->item == el) {return 1;}
		i = i->next;
	}
	return 0;
}
