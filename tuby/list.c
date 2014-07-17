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

void list_init(list *l)
{
    l->first = NULL;
    l->last = NULL;
}

void list_free(list *l)
{
    list_elem *first = l->first;
    while (first != NULL)
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
    le->next = NULL;
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
