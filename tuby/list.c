/* 
 * Copyright (C) 2014 Toth Arpad 
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *    
 *   Toth Arpad (arpytoth@yahoo.com)
 */
#include <stdlib.h>
#include "list.h"

ListElem *list_elem_create()
{
    ListElem *e = (ListElem*)malloc(sizeof(ListElem));
    e->data = NULL;
    e->next = NULL;
    return e;
}

void list_elem_free(ListElem *e)
{
    free(e);
}


List *list_create()
{
    List *newlist = (List *)malloc(sizeof(List));
    list_init(newlist);
    return newlist;
}

void list_init(List *l)
{
    l->first = NULL;
    l->last = NULL;
    l->ref_count = 0;
}

void list_clear(List *l)
{
    ListElem *first = l->first;
    while (first != NULL)
    {
        ListElem *todel = first;
        first = first->next;
        list_elem_free(todel);
    }

}

void list_free(List *l)
{
    list_clear(l);
    free(l);
}

void list_push(List *l, void *e)
{
    ListElem *le = list_elem_create(); 
    le->data = e;
    
    if (l->last == NULL)
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
