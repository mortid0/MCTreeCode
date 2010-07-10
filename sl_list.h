#ifndef SL_LIST_H
#define SL_LIST_H

typedef struct _SL_LIST_ITEM{
	void *item;
	struct _SL_LIST_ITEM *next, *prev;
} SL_LIST_ITEM;

typedef struct _SL_LIST{
	SL_LIST_ITEM *root;
} SL_LIST;


SL_LIST* sl_list_create();

int sl_list_add(SL_LIST *list, void *el);

int sl_list_free(SL_LIST *list);

// delete list element and it's item;
int sl_list_delete(SL_LIST *list, SL_LIST_ITEM *item);

int sl_list_find(SL_LIST *list, void *it);

#endif
