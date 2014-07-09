#include <stdlib.h>
#include "list.h"

void list_init(list *l)
{
    l->first = 0;
    l->last = 0;
}

void list_free(list *l)
{
    list_elem *first = l->first;
    while (first != 0)
    {
        list_elem *todel = first;
        first = first->next;
        free(todel);
    }
}

void list_push(list *l, void *e)
{
    list_elem *le = (list_elem*)malloc(sizeof(list_elem));
    le->data = e;
    if (l->last == 0)
    {
        l->first = le;
        l->last = le;
    }
    else
    {
        l->last->next = le;
        l->last = le;
    }
}
