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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "type_map.h"
#include "vector.h"
#include "allocator.h"
#include "log.h"

int g_alloc_count = 0;

Value *alloc_val(ValueType *val_type)
{
    Value *val = (Value*)malloc(sizeof(Value));
    val->value_type = val_type;
    val->is_null = 1; 
    val->ref_count = 1;
    if (val_type == IntType)
    {
        val->data.int_val = 0;
    }
    if (val_type->is_array)
    {
        vector_init(&val->data.vector_val);
    }
    LOG(llDebug, "Allocated new value: %d", (int)(uintptr_t)val);
    g_alloc_count++;
    return val;
}


Value *alloc_use_val(Value *val)
{
    if (val == NULL)
        return NULL;

    val->ref_count++;
    LOG(llDebug, "Increased count to value: %i to %i", 
            (int)(uintptr_t)val, val->ref_count);
    return val;
}


void alloc_free_val(Value *val)
{
    val->ref_count--;
    LOG(llDebug, "Decreased count to value: %i to %i", 
        (int)(uintptr_t)val, val->ref_count);

    if (val->ref_count == 0)
    {
        if (val->value_type->is_array)
        {
            int i;
            int length = vector_length(&val->data.vector_val);
            for (i = 0; i < length; i++)
            {
                Value *elem = (Value*)vector_at(&val->data.vector_val, i);
                if (elem != 0)
                    alloc_free_val(elem);
            }
        }

        LOG(llDebug, "Freeing value %d", (int)(uintptr_t)val);
        free(val);
        g_alloc_count--;
    }
}


