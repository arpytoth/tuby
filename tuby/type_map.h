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
#ifndef _TYPE_MAP_H_
#define _TYPE_MAP_H_

#include "tuby.h"

////////////////////////////////////////////////////////////////////////////////
// Type Map Structures.
////////////////////////////////////////////////////////////////////////////////

/*
 * An entry from the Type Map.
 */
typedef struct type_map_entry_t
{
    /* Actual type. */
    ValueType *value_type;

    struct type_map_entry_t *next;
} type_map_entry;


/*
 * For now, the type map is a linked list, the searching is slow so do not
 * rely on this structure, and use the actual type_map... functions. This
 * structure will probably change when this will be replaced with an actual
 * map (tree or red-black.. we will see :) )
 */
typedef struct
{
    type_map_entry *first;
    type_map_entry *last;
} type_map;


extern ValueType *VoidType;
extern ValueType *IntType;
extern ValueType *BoolType;

/* Global type map, sort of a singleton, used to store all types by name. */
extern type_map g_type_map;

////////////////////////////////////////////////////////////////////////////////
// Type Map Functions.
////////////////////////////////////////////////////////////////////////////////

/*
 * Used at startup to initialize the global type map.  
 */
void type_map_init();

/*
 * Search for the type with the specified name. If type not defined return
 * NULL.
 */
ValueType *type_map_get(const char *name);

ValueType *typemap_get_array(ValueType *primitive);
ValueType *typemap_get_ref(ValueType *underlying);


/*
 * Put a new type into the type map.
 */
void type_map_put(ValueType *value_type);

/*
 * Releases all types in the type map. Clean type_map_entries so basically
 * delete all allocated objects.
 */
void type_map_release();

#endif //_TYPE_MAP_H_

