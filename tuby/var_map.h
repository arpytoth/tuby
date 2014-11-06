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

#ifndef _VAR_MAP_H_
#define _VAR_MAP_H_

#include "tuby.h"
#include "type_map.h"
#include "allocator.h"


/*
 * An entry from the variables map.
 */
typedef struct VarMapEntryT
{
    /* The actual variable. */
    Var var;

    /* The next varmap entry. If null this is the last element in list. */
    struct VarMapEntryT *next;
} VarMapEntry;


/* 
 * A recursive variables map. It also has a parent. For not this is implemented
 * as a linked list, but in the future this will be a map.
 */
typedef struct VarMapT
{
    VarMapEntry *first;
    VarMapEntry *last;

    /* If this have a parent, all searches must go to the parent. */
    struct VarMapT *parent;
} VarMap;


/*
 * Initialize the global variables map.
 */
void varmap_init();

/*
 * Get variable in the current scope by name, or return NULL if variable not
 * found.
 */
Var *varmap_get(const char *name);

/*
 * Define a new variable with the specified name, and return the variable.  
 */
Var *varmap_def(const char *name, ValueType *val_type);

/*
 * Release variables map and free all allocated variables.
 */
void varmap_release();

/*
 * Create a new varmap an push it into the current varmap.
 */
VarMap *varmap_push();

/*
 * Pops the current varmap.. and release all variables.
 */
VarMap *varmap_pop();

/*
 * If current map is not the global varmap, simply purge the hashmap. 
 */
void varmap_purge();

void varmap_free_var(struct Var *var);
#endif // _VAR_MAP_H_
