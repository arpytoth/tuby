#ifndef _VAR_MAP_H_
#define _VAR_MAP_H_

#include "ast.h"

typedef struct VarMapEntryT
{
    /* The actual variable. */
    Var var;

    /* Name of this variable. */
    char *name;

    /* The next varmap entry. If null this is the last element in list. */
    struct VarMapEntryT *next;
} VarMapEntry;

/* A recursive variables map. It also has a parent. */
typedef struct VarMapT
{
    VarMapEntry *first;
    VarMapEntry *last;
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
Var *varmap_def(const char *name);

/*
 * Release variables map and free all allocated variables.
 */
void varmap_release();

#endif // _VAR_MAP_H_
