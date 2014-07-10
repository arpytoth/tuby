#ifndef _TYPE_MAP_H_
#define _TYPE_MAP_H_

#include "ast.h"

////////////////////////////////////////////////////////////////////////////////
// Type Map Structures.
////////////////////////////////////////////////////////////////////////////////

/*
 * An entry from the Type Map.
 */
typedef struct type_map_entry_t
{
    /* Name of this type. */
    char *name;

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


/* The primitive integer type. */
extern ValueType *IntType;

/* The primitive boolean type.*/
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


/*
 * Put a new type into the type map.
 */
void type_map_put(const char *name, ValueType *value_type);

/*
 * Releases all types in the type map. Clean type_map_entries so basically
 * delete all allocated objects.
 */
void type_map_release();

#endif //_TYPE_MAP_H_

