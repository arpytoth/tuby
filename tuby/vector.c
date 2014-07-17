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
#include "vector.h"

void vector_init(vector *v)
{
    v->data = 0;
    v->reserved = 0;
    v->size = 0;
}


void vector_release(vector *v)
{
    free(v->data);
}


void *vector_at(vector *v, int index)
{
    return v->data[index];
}


int vector_length(vector *v)
{
    return v->size;
}

void vector_reserve(vector *v, int size)
{
    if (size > v->reserved)
    {
        int size_of_pointer = sizeof(void*);
        int new_size = v->reserved == 0 ? 1 : v->reserved * 2;
        v->data = (void**)realloc(v->data, new_size * size_of_pointer);
        v->reserved = new_size;
    }
}


void vector_push(vector *v, void *elem)
{
    int size_of_pointer = sizeof(void*);
    if (v->reserved - v->size == 0)
    {
        int new_size = v->reserved == 0 ? 1 : v->reserved * 2;
        v->data = (void**)realloc(v->data, new_size * size_of_pointer);
        v->reserved = new_size;
    }
    v->size++;
    v->data[v->size - 1] = elem;
}
