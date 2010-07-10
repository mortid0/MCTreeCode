#include "sl_list.h"
#include <stdlib.h>
#include <stdio.h>


SL_LIST* sl_list_create()
{
	SL_LIST * list = malloc(sizeof(SL_LIST));
	list->root = NULL;
	return list;
}

//adding in the head of list;
int sl_list_add(SL_LIST *list, void *el)
{
	SL_LIST_ITEM *it = malloc(sizeof(SL_LIST_ITEM));
	if (NULL == it) {printf("Out of space\n");exit(-1);}
	it->next = list->root;
	if (NULL != list->root) {list->root->prev = it;}
	it->item = el;
	it->prev = NULL;
	list->root = it;
	return 0;
}

int sl_list_free(SL_LIST *list)
{
	while (NULL != list->root)
	{
		sl_list_delete(list, list->root);
	}
	free(list);
	return 0;
}

int sl_list_delete(SL_LIST *list, SL_LIST_ITEM *item)
{
	SL_LIST_ITEM *prv = item->prev, *nxt = item->next;
	if (NULL == prv) {list->root = nxt; nxt->prev = NULL;}                  // root element
	else {prv->next = nxt;}
	if (nxt != NULL) { nxt->prev = prv;}
	free(item->item);
	free(item);
	return 0;
}

int sl_list_find(SL_LIST *list, void *it)
{
	SL_LIST_ITEM *curr = list->root;
	while(NULL != curr)
	{
		if (curr->item == it) {return 1;}
		curr = curr->next;
	}
	return 0;
}



