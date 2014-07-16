#ifndef _VAR_MAP_H_
#define _VAR_MAP_H_

#include "ast.h"

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

#endif // _VAR_MAP_H_
