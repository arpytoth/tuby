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

#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct
{
    void **data;  /* Contains the data in this vector.*/
    int size;     /* Size of this vector. */
    int reserved; /* Number of reserved items.*/
    int ref_count; /* Used if this vector is shared among multiple Values*/
} vector;

void vector_init(vector *vector);
void vector_release(vector *vector);
void *vector_at(vector *v, int index);
void *vector_set_at(vector *v, int index, void *elem);
int vector_length(vector *v);
void vector_reserve(vector *v, int size);
void vector_resize(vector *v, int newsize);
void vector_push(vector *v, void *elem);

#endif // _VECTOR_H_
