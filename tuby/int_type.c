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

#include "int_type.h"
#include "func_table.h"
#include "stack.h"

////////////////////////////////////////////////////////////////////////////////
//                         NATIVE FUNCTIONS ON INT                            //
////////////////////////////////////////////////////////////////////////////////

void int_print()
{
    Value *param1 = stack_function_param(0);
    if (param1->value_type == IntType)
    {
        int int_val = param1->data.int_val;
        printf("%d\n", int_val);
    }
    stack_set_ret_val(NULL);
}

void int_inc_operator()
{
    Value *param1 = stack_function_param(0);
    int int_val1 = param1->data.int_val;
    Value *ret_val = alloc_val(IntType);
    ret_val->data.int_val = int_val1 + 1;
    stack_set_ret_val(ret_val);
}

void int_add_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(IntType);
    ret_val->data.int_val = int_val1 + int_val2;
    stack_set_ret_val(ret_val); 
}

void int_sub_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(IntType);
    ret_val->data.int_val = int_val1 - int_val2;
    stack_set_ret_val(ret_val); 
}

void int_equals_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(BoolType);
    ret_val->data.bool_val = int_val1 == int_val2;
    stack_set_ret_val(ret_val); 

}

void int_not_equals_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(BoolType);
    ret_val->data.bool_val = int_val1 != int_val2;
    stack_set_ret_val(ret_val); 
}

void int_less_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(BoolType);
    ret_val->data.bool_val = int_val1 < int_val2;
    stack_set_ret_val(ret_val); 
}

void int_mul_operator()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_val(IntType);
    ret_val->data.int_val = int_val1 * int_val2;
    stack_set_ret_val(ret_val); 
}

void int_assign_operator()
{
    Value *dest = stack_function_param(0);
    Value *src = stack_function_param(1);
    int int_val2 = src->data.int_val;
    dest->data.int_val = int_val2;   
    stack_set_ret_val(NULL); 
}

////////////////////////////////////////////////////////////////////////////////
//                              INT RELATED UTILS                             //
////////////////////////////////////////////////////////////////////////////////

void int_init_module()
{
    FuncDef *func;

    // int = int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("=");
    func->native = int_assign_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 1));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // print(int)
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("print");
    func->native = int_print;
    func->params = (vector*)malloc(sizeof(vector));
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // int + int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("+");
    func->native = int_add_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // int - int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("-");
    func->native = int_sub_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

     // int * int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("*");
    func->native = int_mul_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // int == int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("==");
    func->native = int_equals_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = BoolType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // int != int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("!=");
    func->native = int_not_equals_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = BoolType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);
    
    // int < int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("<");
    func->native = int_less_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = BoolType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // int++;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("++");
    func->native = int_inc_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);
}
