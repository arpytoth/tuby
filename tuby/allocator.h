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

#include "type_map.h"
#include "tuby_array.h"
#include "tuby_string.h"

struct Value;
typedef struct Value Value;

union AllValues
{
    int int_val;
    int bool_val;
    char char_val;
    struct Array array_val;
    struct String str_val;
};


/*
 * A value in the Tuby program, basically a meaningfull chunk of memory such as
 * an integer value or a string. Expressions are evaluated into values and 
 * stored into the memory.
 */
struct Value
{
    ValueType *value_type;
    union AllValues data;
    int ref_count;
    int is_null;

    /*
     * The value this value is part of. For example if this value is
     * part of an array or a string, the parent_value will be the
     * array or string value.
     */
    struct Value *parent_value;
};




/* Used to count how many objects where allocated and dealocted.*/
extern int g_alloc_count;

/*
 * Allocate a new value of the specified value type.
 */
Value *alloc_val(ValueType *type);

/*
 * Allocate a new value on the Heap.
 */
Value *alloc_use_val(Value *val);

/*
 * Free value from the Heap, but only if ref_count is zero. 
 */
void alloc_free_val(Value *val);
Value *alloc_array_get(Value *a, int index);
#endif // _ALLOCATOR_H_

