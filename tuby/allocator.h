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

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "ast.h"


/* Used to count how many objects where allocated and dealocted.*/
extern int g_alloc_count;

/*
 * Allocate a new value of the specified value type.
 */
Value *alloc_val(ValueType *type);

/*
 * Allocate a new value on the Heap.
 */
Value *alloc_get_val(Value *val);

/*
 * Free value from the Heap, but only if ref_count is zero. 
 */
void alloc_free_val(Value *val);


#endif // _ALLOCATOR_H_

