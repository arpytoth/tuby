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

#ifndef _LIST_H_
#define _LIST_H_

/* An element in the linked list. */
typedef struct list_elem_t
{
    void *data; /* Actual data stored in this list. */
    struct list_elem_t *next; /* Pointer to the next element in the list.*/
} list_elem;

/* Structure that holds the data of a linked list. */
typedef struct list_t
{
    struct list_elem_t *first;
    struct list_elem_t *last;
} list;

/* Initialize a new Linked List. */
void list_init(list *l);

/* Release data held by this Linked List. */
void list_free(list *l);

/* Push a new element at the end of this list. */
void list_push(list *l, void *e);


#endif // _LIST_H_
