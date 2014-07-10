#include <stdlib.h>
#include <string.h>
#include "type_map.h"

ValueType *IntType;
ValueType *BoolType;
type_map g_type_map;


void type_map_init()
{
    g_type_map.first = NULL;
    g_type_map.last = NULL;

    IntType = (ValueType*)malloc(sizeof(ValueType));
    BoolType = (ValueType*)malloc(sizeof(ValueType));

   type_map_put("int", IntType);
   type_map_put("boolean", BoolType);
}


ValueType *type_map_get(const char *name)
{
    type_map_entry *e = g_type_map.first;
    while (e != NULL)
    {
        if (strcmp(e->name, name) == 0)
            return e->value_type;
        e = e->next;
    }
    return NULL;
}


void type_map_put(const char *name, ValueType *value_type)
{
    type_map_entry *e = (type_map_entry*)malloc(sizeof(type_map_entry));
    e->next = NULL;
    e->name = (char*)malloc(strlen(name)+1);
    strcpy(e->name, name);
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

