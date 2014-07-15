#include <string.h>
#include <stdlib.h>
#include "var_map.h"

VarMap *g_varmap;

//------------------------------------------------------------------------------

void varmap_init()
{
    g_varmap = (VarMap*)malloc(sizeof(VarMap));
    g_varmap->first = NULL;
    g_varmap->last = NULL;
    g_varmap->parent = NULL;
}

//------------------------------------------------------------------------------

Var *varmap_get(const char *name)
{
    if (g_varmap->first != NULL)
    {
        VarMapEntry *e = g_varmap->first;
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
   
    if (g_varmap->last != NULL)
    {
        g_varmap->last->next = e;
        g_varmap->last = e;
    }
    else
    {
        g_varmap->last = e;
        g_varmap->first = e;
    }
    return &e->var;
}

//------------------------------------------------------------------------------

void varmap_release()
{
    if (g_varmap->first != NULL)
    {
        VarMapEntry *e = g_varmap->first;
        while (e != NULL)
        {
            VarMapEntry *next = e->next;
            free(e->var.name);
            free(e);
            e = next;
        }
    }
    g_varmap->first = NULL;
    g_varmap->last = NULL;
}


VarMap *varmap_push()
{
    if (g_varmap == NULL)
    {
        g_varmap = (VarMap*)malloc(sizeof(VarMap));
        g_varmap->first = NULL;
        g_varmap->last = NULL;
        g_varmap->parent = NULL;
    }
    else
    {
        VarMap *new = NULL;
        new = (VarMap*)malloc(sizeof(VarMap));
        new->first = NULL;
        new->last = NULL;
        new->parent = g_varmap;
        g_varmap = new;
    }
    return g_varmap;
}


VarMap *varmap_pop()
{
    if (g_varmap == NULL)
    {
        return g_varmap;
    }
    else
    {
        VarMap *to_pop = g_varmap;
        g_varmap = g_varmap->parent;
        return to_pop;
    }
}


