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

typedef struct ListElem
{
    void *data; 
    struct ListElem *next;
} ListElem;

ListElem *list_elem_create();
void list_elem_free(ListElem *elem);


typedef struct List
{
    struct ListElem *first;
    struct ListElem *last;
    int ref_count;
} List;

List *list_create();
void  list_init(List *l);
void  list_clear(List *l);
void  list_free(List *l);
void  list_push(List *l, void *e);

#endif // _LIST_H_
