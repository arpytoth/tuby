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
#include "stack.h"
#include "utils.h"

/* The execution stack used by the Tuby program. */
Stack g_stack;

//------------------------------------------------------------------------------

void stack_init()
{
    static int STACK_SIZE = 100;
    g_stack.pos = 0;
    g_stack.size = STACK_SIZE;
    g_stack.data = (StackFrame*)malloc(sizeof(StackFrame) * STACK_SIZE);
}

//------------------------------------------------------------------------------

StackFrame *stack_push()
{
    if (g_stack.pos < g_stack.size - 1)
    {
        g_stack.pos++;
        return &g_stack.data[g_stack.pos];
    }
    return NULL;
}

//------------------------------------------------------------------------------

StackFrame *stack_pop()
{
    if (g_stack.pos >= 0)
    {
        g_stack.pos--;
        return &g_stack.data[g_stack.pos + 1];
    }
    return NULL;
}

//------------------------------------------------------------------------------

StackFrame *stack_top()
{
    if (g_stack.pos >= 0)
    {
        return &g_stack.data[g_stack.pos];
    }
    return NULL;
}

//------------------------------------------------------------------------------

Value *stack_function_param(int index)
{
    StackFrame *top = stack_top();
    if (top != NULL)
    {
        return (Value*)vector_at(&top->params, index);
    }
    else
    {
        return NULL;
    }
}


void stack_set_ret_val(Value *val)
{
    StackFrame *top = stack_top();
    if (top != NULL)
    {
        top->ret_val = val;
    }
}


Value *stack_get_ret_val()
{
    StackFrame *top = stack_top();
    if (top != NULL)
    {
        return top->ret_val;
    }

    return NULL;
}

