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

////////////////////////////////////////////////////////////////////////////////
//                         NATIVE FILE FUNCTIONS                              //
////////////////////////////////////////////////////////////////////////////////

void file_fopen()
{
    Value *filename_param = stack_function_param(0);
    Value *mode_param = stack_function_param(1);
    
    Value *fileid = alloc_val(IntType);
    fileid->data.int_val = 0;
    stack_set_ret_val(fileid);
}

////////////////////////////////////////////////////////////////////////////////
//                                FILE MODULE                                 //
////////////////////////////////////////////////////////////////////////////////

void file_init_module()
{
    FuncDef *func;

    // fopen
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("fopen");
    func->native = file_fopen;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, new_param_info(StrType, 0));
    vector_push(func->params, new_param_info(StrType, 0));
    func_def(func);
}

