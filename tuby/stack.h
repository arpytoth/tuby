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

#ifndef _STACK_H_
#define _STACK_H_

#include "vector.h"
#include "ast.h"

/* A frame in The Stack. Holds the informations about a function call. */
typedef struct 
{
    /* List of called parameters. */
    vector params;
    
    /* 
     * When a subprogram exists, this is where the value will be 
     * stored
     */
    Value *ret_val;
} StackFrame;

/* The execution stack. */
typedef struct StackT
{
    /* Size of the stack. */
    int size;

    /* Current position of the top of the stack. */
    int pos;
    
    /* Preallocated data in this stack. */
    StackFrame *data;
} Stack;

/*
 * Initialize the execution stack.
 */
void stack_init();

/*
 * Pop last frame from the top.
 */
StackFrame *stack_pop();

/*
 * Push a new frame on the stack and return it.
 */
StackFrame *stack_push();

/*
 * Get the top element of the stack without actually removing it. To remove it
 * is stack_pop() function. If NULL is returned, it means the stack is empty.
 */
StackFrame *stack_top();

Value *stack_function_param(int index);

/*
 * Set the return value for the current stack frame.
 */
void stack_set_ret_val(Value *val);

/*
 * Get the return value for the current stack frame.
 */
Value *stack_get_ret_val();
#endif // _STACK_H_

