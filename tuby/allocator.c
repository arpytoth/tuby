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

#include "vector.h"
#include "allocator.h"
#include "log.h"


Value *alloc_val(ValueType *val_type)
{
    Value *val = (Value*)malloc(sizeof(Value));
    val->value_type = val_type;
    val->ref_count = 1;
    if (val_type->is_array)
    {
        vector_init(&val->data.vector_val);
    }
    LOG(llDebug, "Allocated new value: %d", (int)(uintptr_t)val);
    return val;
}


Value *alloc_get_val(Value *val)
{
    if (val == NULL)
    {
        val = (Value*)malloc(sizeof(Value));
        val->ref_count = 1;
        LOG(llDebug, "Allocated new value: %d", (int)(uintptr_t)val);
    }
    else
    {
        val->ref_count++;
        LOG(llDebug, "Increased count to value: %i to %i", 
            (int)(uintptr_t)val, val->ref_count);
    }
    return val;
}


void alloc_free_val(Value *val)
{
    val->ref_count--;
    LOG(llDebug, "Decreased count to value: %i to %i", 
        (int)(uintptr_t)val, val->ref_count);

    if (val->ref_count == 0)
    {
        LOG(llDebug, "Freeing value %d", (int)(uintptr_t)val);
        if (val
        free(val);
    }
}


