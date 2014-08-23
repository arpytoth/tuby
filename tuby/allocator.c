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

#include "tuby_string.h"
#include "type_map.h"
#include "vector.h"
#include "allocator.h"
#include "log.h"
#include "tuby_array.h"

int g_alloc_count = 0;

Value *alloc_val(ValueType *val_type)
{
    Value *val = (Value*)malloc(sizeof(Value));
    val->value_type = val_type;
    val->is_null = 1; 
    val->ref_count = 1;
    val->parent = NULL;
    if (val_type == IntType)
    {
        val->data.int_val = 0;
    }
    if (val_type == StrType)
    {
        string_init(&val->data.str_val);
    }
    if (val_type->is_array)
    {
        array_init(&val->data.array_val);
    }
    LOG(llDebug, "Allocated new value: %d", (int)(uintptr_t)val);
    g_alloc_count++;
    return val;
}

struct Value *alloc_by_value(struct Value *orig)
{
    struct Value *clone = alloc_val(orig->value_type);
    if (orig->value_type->is_array)
    {
        array_copy(&clone->data.array_val, &orig->data.array_val);
    }
    else
    {
        clone->data = orig->data;
    }
    clone->is_null = orig->is_null;
    return clone;
}

Value *alloc_use_val(Value *val)
{
    if (val == NULL)
        return NULL;

    val->ref_count++;
    return val;
}


void alloc_free_val(Value *val)
{
    val->ref_count--;
    LOG(llDebug, "Decreased count to value: %i to %i", 
        (int)(uintptr_t)val, val->ref_count);

    if (val->ref_count == 0)
    {
        if (val->value_type == StrType)
        {
            string_free(&val->data.str_val);
        }
        else if (val->value_type->is_array)
        {
            array_free(&val->data.array_val);
        }

        LOG(llDebug, "Freeing value %d", (int)(uintptr_t)val);
        free(val);
        g_alloc_count--;
    }
}

Value *alloc_array_get(Value *a, int index)
{
    Value *val = NULL;
    vector_resize(a->data.array_val.data, index + 1);
    
    val = vector_at(a->data.array_val.data, index);
    if (val == NULL)
    {
        val = alloc_val(a->value_type->uval_type);
        vector_set_at(a->data.array_val.data, index, val);
    }
    return val;
}


