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
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "func_table.h"
#include "stack.h"
#include "redblack.h"

RedBlackTree *g_file_map;
int g_file_seed;

int file_map(FILE *fp)
{
    if (fp == NULL)
        return -1;
    g_file_seed++;
    irb_insert(g_file_map, g_file_seed, fp);
    return g_file_seed;
}

FILE *file_get(int id)
{
    FILE *fp = (FILE*)irb_search(g_file_map, id);
    return fp;
}
////////////////////////////////////////////////////////////////////////////////
//                         NATIVE FILE FUNCTIONS                              //
////////////////////////////////////////////////////////////////////////////////

void file_fopen()
{
    Value *filename_param = stack_function_param(0);
    Value *mode_param = stack_function_param(1);
    char *filename = filename_param->data.str_val.buffer;
    char *mode = mode_param->data.str_val.buffer;
    FILE *fp = fopen(filename, mode);
    int id = file_map(fp);
    Value *fileid = alloc_val(IntType);
    fileid->data.int_val = id;
    stack_set_ret_val(fileid);
}

void file_fprintf()
{
    Value *file_param = stack_function_param(0);
    Value *text_param = stack_function_param(1);
    int fileid = file_param->data.int_val;
    char *text =text_param->data.str_val.buffer;
    FILE *fp = file_get(fileid);
    int size = -1;
    if (fp != NULL)
        size = fprintf(fp, "%s", text);

    Value *res = alloc_val(IntType);
    res->data.int_val = size;
    stack_set_ret_val(res);

}

void file_fread()
{   
    Value *file_param = stack_function_param(0);
    Value *size_param = stack_function_param(1);
    
    int fileid = file_param->data.int_val;
    int size  =text_param->data.int_val;
    FILE *fp = file_get(fileid);
    int size = -1;
    
    char buffer[255]; // TODO dynamic allocation.
    if (fp != NULL)
        size = fread(buffer, 0, 250, fp);

    Value *res = alloc_val(IntType);
    res->data.int_val = size;
    stack_set_ret_val(res);
}

////////////////////////////////////////////////////////////////////////////////
//                                FILE MODULE                                 //
////////////////////////////////////////////////////////////////////////////////

void file_init_module()
{
    g_file_map = irb_new();
    g_file_seed = 0;

    FuncDef *func;

    // fopen
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("fopen");
    func->native = file_fopen;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);
 
    // fprintf(string)
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("fprintf");
    func->native = file_fprintf;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(IntType, 0));
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);

}

