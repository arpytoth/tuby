#include <string.h>
#include <stdlib.h>
#include "var_map.h"

VarMap g_varmap;

//------------------------------------------------------------------------------

void varmap_init()
{
    g_varmap.first = NULL;
    g_varmap.last = NULL;
}

//------------------------------------------------------------------------------

Var *varmap_get(const char *name)
{
    if (g_varmap.first != NULL)
    {
        VarMapEntry *e = g_varmap.first;
        while (e != NULL)
        {
            if (strcmp(e->var.name, name) == 0)
            {
                return &e->var;
            }
            e = e->next;
        }
        return NULL;
    }
    else
    {
        return NULL;
    }
}

//------------------------------------------------------------------------------

Var *varmap_def(const char *name, ValueType *val_type)
{   
    VarMapEntry *e = (VarMapEntry *)malloc(sizeof(VarMapEntry));
    e->var.name = (char *)malloc(strlen(name) + 1);
    strcpy(e->var.name, name);
    e->next = NULL;
    e->var.val_type = val_type;
   
    if (g_varmap.last != NULL)
    {
        g_varmap.last->next = e;
        g_varmap.last = e;
    }
    else
    {
        g_varmap.last = e;
        g_varmap.first = e;
    }
    return &e->var;
}

//------------------------------------------------------------------------------

void varmap_release()
{
    if (g_varmap.first != NULL)
    {
        VarMapEntry *e = g_varmap.first;
        while (e != NULL)
        {
            VarMapEntry *next = e->next;
            free(e->var.name);
            free(e);
            e = next;
        }
    }
    g_varmap.first = NULL;
    g_varmap.last = NULL;
}

//------------------------------------------------------------------------------


