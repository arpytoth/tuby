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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tuby_char.h"
#include "func_table.h"
#include "ast.h"
#include "stack.h"
#include "utils.h"

ValueType *CharType;

void char_print_func()
{
    Value *param1 = stack_function_param(0);
    if (param1->value_type == CharType)
    {
        printf("%c\n", param1->data.char_val);
    }    
    else
    {
       error("Not A Char! FIX THIS!\n");
    }
}

void char_assign_operator()
{
    struct Value *lvalue= stack_function_param(0);
    struct Value *rvalue= stack_function_param(1);

    lvalue->data.char_val = rvalue->data.char_val;
    lvalue->is_null = rvalue->is_null;
    stack_set_ret_val(NULL); 
}

void char_init_module()
{
    FuncDef *func = NULL;

    CharType = (ValueType*)malloc(sizeof(ValueType));
    CharType->uval_type = NULL;
    CharType->name = strdup("char");
    type_map_put(CharType);

    
    // Built In Functions
    
    // print(char);
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("print");
    func->native = char_print_func;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, CharType);
    func_def(func);

    // char = char
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("=");
    func->native = char_assign_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, CharType);
    vector_push(func->params, CharType);
    func_def(func);
}
