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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tuby_char.h"
#include "tuby_string.h"
#include "func_table.h"
#include "utils.h"
#include "stack.h"

ValueType *StrType;

void string_square_operator()
{
    struct Value *string = stack_function_param(0);
    struct Value *index = stack_function_param(1);
    int int_index = index->data.int_val;
    struct Value *char_at_index = NULL;
    struct String *str_val = &string->data.str_val;

    char_at_index = alloc_val(CharType);
    char_at_index->data.char_val = string_at(str_val, int_index);
    char_at_index->parent = string;
    char_at_index->index = int_index;
    stack_set_ret_val(char_at_index); 
}

void string_add_operator()
{
    // doing the append operator.
    // string + string
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    
    String *val1 = &param1->data.str_val;
    String *val2 = &param2->data.str_val;

    Value *ret_val = alloc_val(StrType);
    string_concat(&ret_val->data.str_val, val1, val2);   
    stack_set_ret_val(ret_val); 
}

void str_add_int_operator()
{
    // string + int
    char buffer[20];
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    
    String *str_val = &param1->data.str_val;
    int int_val = param2->data.int_val;

    sprintf(buffer, "%d", int_val);
    Value *ret_val = alloc_val(StrType);
    ret_val->data.str_val.len = strlen(buffer) + str_val->len;
    ret_val->data.str_val.buffer = (char*)malloc(ret_val->data.str_val.len + 1);
    strcat(ret_val->data.str_val.buffer, str_val->buffer);
    strcat(ret_val->data.str_val.buffer, buffer);
    stack_set_ret_val(ret_val); 
}

void string_assign_operator()
{
    struct Value *lvalue= stack_function_param(0);
    struct Value *rvalue= stack_function_param(1);

    string_dup(&lvalue->data.str_val, &rvalue->data.str_val);
    lvalue->is_null = rvalue->is_null;
    stack_set_ret_val(NULL); 
}

void string_print_func()
{
    Value *param1 = stack_function_param(0);
    if (param1->value_type == StrType)
    {
        struct String val = param1->data.str_val;
        printf("%s\n", val.buffer);
    }    
    else
    {
       error("Not A String! FIX THIS!\n");
    }
}

void string_init_module()
{
    FuncDef *func = NULL;

    StrType = (ValueType*)malloc(sizeof(ValueType));
    StrType->name = strdup("string");
    StrType->uval_type = CharType;
    type_map_put(StrType);

    // Built In Functions
    // print(string);
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("print");
    func->native = string_print_func;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);

    // string = string;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("=");
    func->native = string_assign_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 1));
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);
  
    // string + string;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("+");
    func->native = string_add_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = StrType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);
    
    // string + int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("+");
    func->native = str_add_int_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = StrType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    vector_push(func->params, new_param_info(IntType, 0));
    func_def(func);

    // string[int];
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("[]");
    func->native = string_square_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = CharType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    vector_push(func->params, new_param_info(IntType, 1));
    func_def(func);

}


void string_init(struct String *str)
{
    str->len = 0;
    str->buffer = strdup("");
}


int string_len(struct String *str)
{
    return str->len;
}


void string_set(struct String *str, const char *val)
{
   int len = strlen(val);
   if (str->buffer != NULL)
       free(str->buffer);
   str->buffer = strdup(val);
   str->len = len;
}

void string_dup(struct String *dest, struct String *src)
{
    if (dest->buffer != NULL)
        free(dest->buffer);
    
    dest->buffer = strdup(src->buffer);
    dest->len = src->len;
}

void string_concat(String *dest, String *str1, String *str2)
{
    if (dest->buffer != NULL)
        free(dest->buffer);
    dest->len = str1->len + str2->len;
    dest->buffer = (char *)malloc(dest->len + 1);
    dest->buffer[0] = '\0';
    strcat(dest->buffer, str1->buffer);
    strcat(dest->buffer, str2->buffer);
}

void string_free(struct String *str)
{
    free(str->buffer);
}


char string_at(struct String *str, int index)
{
    return str->buffer[index];
}

