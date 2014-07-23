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
#include <string.h>
#include "type_map.h"
#include "string.h"

ValueType *IntType;
ValueType *BoolType;
type_map g_type_map;


void type_map_init()
{
    g_type_map.first = NULL;
    g_type_map.last = NULL;

    IntType = (ValueType*)malloc(sizeof(ValueType));
    IntType->name = strdup("int");

    BoolType = (ValueType*)malloc(sizeof(ValueType));
    BoolType->name = strdup("boolean");

    type_map_put(IntType);
    type_map_put(BoolType);
}


ValueType *type_map_get(const char *name)
{
    type_map_entry *e = g_type_map.first;
    while (e != NULL)
    {
        if (strcmp(e->value_type->name, name) == 0)
            return e->value_type;
        e = e->next;
    }
    return NULL;
}


ValueType *typemap_get_array(ValueType *primitive)
{
    char *arrayname = str_concat(primitive->name, "[]");
    ValueType *type = type_map_get(arrayname);

    if (type == NULL)
    {
        type = (ValueType*)malloc(sizeof(ValueType));
        type->name = arrayname;
        type->uval_type = primitive;
        type->is_array = 1;
        type_map_put(type);
        return type;
    }
    else
    {
        free(arrayname);
        return type;
    }
}


void type_map_put(ValueType *value_type)
{
    type_map_entry *e = (type_map_entry*)malloc(sizeof(type_map_entry));
    e->next = NULL;
    e->value_type = value_type;

    if (g_type_map.last == NULL)
    {
        g_type_map.last = e;
        g_type_map.first = e;
    }
    else
    {
        g_type_map.last->next = e;
        g_type_map.last = e;
    }
}


void type_map_release()
{
    // TODO  please.. implement me :)
}

