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
#include <stdio.h>

#include "utils.h"
#include "redblack.h"
#include "type_map.h"
#include "func_table.h"
#include "stack.h"
#include "allocator.h"
#include "ast.h"

struct FuncTableEntry
{
    FuncDef *function;
    struct FuncTableEntry *next;
};

struct FuncTable
{
    struct FuncTableEntry *first;
    struct FuncTableEntry *last;
};

struct FuncTable g_func_table;

void func_init()
{
    g_func_table.first = NULL;
    g_func_table.last = NULL;
}


FuncDef *func_search(FuncDef *f)
{
    struct FuncTableEntry *e = g_func_table.first;
    while (e != 0)
    {
        if (strcmp(f->name, e->function->name) == 0)
        {
            if (vector_length(f->params) == vector_length(e->function->params))
            {   
                char ok = 1;
                int length = vector_length(f->params);
                int i;
                
                for (i = 0; i < length; i++)
                {
                     struct ParamInfo *t1 = vector_at(e->function->params, i);
                     struct ParamInfo *t2 = vector_at(f->params, i);
                     if (t1->value_type != t2->value_type)
                     {
                        ok = 0;
                        break;
                     }     
                }
                
                if (ok == 1)
                    return e->function;
            }
        }

        e = e->next;
    }
    return 0;
}


void func_def(FuncDef *func)
{
    FuncDef *f = func_search(func);
    if (f == NULL)
    {
        struct FuncTableEntry *entry;

        entry = (struct FuncTableEntry*)malloc(sizeof(struct FuncTableEntry));

        entry->function = func;
        entry->next = 0;
        if (g_func_table.last == 0)
        {
            g_func_table.last = entry;
            g_func_table.first = entry;
        }
        else
        {
            g_func_table.last->next = entry;
            g_func_table.last = entry;
        }
    }
    else
    {
        parse_error("Function %s already defined.", func->name);
    }
}





FuncDef *func_get(const char *name, vector *params)
{
    struct FuncTableEntry *e = g_func_table.first;
    while (e != 0)
    {
        if (strcmp(name, e->function->name) == 0)
        {
            if (vector_length(params) == vector_length(e->function->params))
            {   
                char ok = 1;
                int length = vector_length(params);
                int i;
                
                for (i = 0; i < length; i++)
                {
                     struct ParamInfo *t1 = vector_at(e->function->params, i);
                     AstNode *expr = vector_at(params, i);
                     ValueType *t2 = expr->value_type;  
                     if (t1->value_type != t2)
                     {
                        ok = 0;
                        break;
                     }     
                }
                
                if (ok == 1)
                    return e->function;
            }
        }

        e = e->next;
    }
    return 0;
}

struct ParamInfo *new_param_info(struct ValueType *value_type, 
    char is_reference)
{
    struct ParamInfo *param_info = NULL;
    param_info = (struct ParamInfo*)malloc(sizeof(struct ParamInfo));
    param_info->value_type = value_type;
    param_info->is_reference = is_reference;
    return param_info;
}


