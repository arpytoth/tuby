#include <string.h>
#include <stdlib.h>

#include "var_map.h"
#include "utils.h"
#include "allocator.h"

VarMap *g_varmap;

//------------------------------------------------------------------------------

void varmap_init()
{
    g_varmap = (VarMap*)malloc(sizeof(VarMap));
    g_varmap->first = NULL;
    g_varmap->last = NULL;
    g_varmap->parent = NULL;
}


Var *varmap_get_rec(VarMap *map, const char *name)
{
    if (map->first != NULL)
    {
        VarMapEntry *e = map->first;
        while (e != NULL)
        {
            if (strcmp(e->var.name, name) == 0)
            {
                return &e->var;
            }
            e = e->next;
        }
        // Recursivly search the parent scopes :)
        if (map->parent != NULL)
        {
            return varmap_get_rec(map->parent, name);
        }
        return NULL;
    }
    else
    {
        return NULL;
    }

}


Var *varmap_get(const char *name)
{
    return varmap_get_rec(g_varmap, name);
}

//------------------------------------------------------------------------------

Var *varmap_def(const char *name, ValueType *val_type)
{   
    Var *var = varmap_get(name);
    if (var == NULL)
    {
        VarMapEntry *e = (VarMapEntry *)malloc(sizeof(VarMapEntry));
        e->var.name = (char *)malloc(strlen(name) + 1);
        strcpy(e->var.name, name);
        e->next = NULL;
        e->var.val_type = val_type;
        e->var.val = NULL;

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
    else
    {
        parse_error("Variable %s already defined before", name);
        return NULL;
    }
}


void varmap_rel(VarMap *amap)
{
    if (amap->first != NULL)
    {
        VarMapEntry *e = amap->first;
        while (e != NULL)
        {
            VarMapEntry *next = e->next;
            free(e->var.name);
            if (e->var.val != NULL)
                alloc_free_val(e->var.val);

            free(e);
            e = next;
        }
    }
    amap->first = NULL;
    amap->last = NULL;
}


void varmap_release()
{
    if (g_varmap != NULL)
    {
        varmap_rel(g_varmap);
    }
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


void varmap_purge()
{
    VarMap *map = varmap_pop();
    if (map != NULL)
    {
        varmap_rel(map);
        free(map);
    }
}
