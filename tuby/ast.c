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
#include "ast.h"
#include "type_map.h"
#include "func_table.h"
#include "utils.h"

AstNode *ast_binary(const char *symbol, AstNode *t1, AstNode *t2)
{
    AstNode *func_call_node = NULL;
    FuncCall *func_call = NULL;
    ValueType *ret_type = NULL;

    func_call_node = (AstNode*)malloc(sizeof(AstNode));
    func_call_node->type = antFuncCall;
    func_call = &func_call_node->content.func_call;
    vector_init(&func_call->params);
    vector_push(&func_call->params, t1);
    vector_push(&func_call->params, t2);

    func_call_node->content.func_call.func = 
        func_get(symbol, &func_call->params);

    if (func_call_node->content.func_call.func == NULL) 
    {
        parse_error("Operator + not defined for arguments of type %s "
                    "and %s", t1->value_type == 0 ? "void":t1->value_type->name,
                    t2->value_type == 0 ? "void" : t2->value_type->name);
    }

    ret_type = func_call_node->content.func_call.func->value_type;
    func_call_node->value_type = ret_type; 
    return func_call_node;
}


AstNode *ast_bool_val(int value)
{
    AstNode *bool_val_node = (AstNode*)malloc(sizeof(AstNode));

    bool_val_node->content.bool_val = value;
    bool_val_node->type = antBoolVal;
    bool_val_node->value_type = BoolType;
    return bool_val_node;
}


AstNode *ast_var_decl(char *name, ValueType *val_type)
{
    AstNode *var_decl_node = (AstNode*)malloc(sizeof(AstNode));
    var_decl_node->type = antVarDecl;
    var_decl_node->content.var_decl.name = (char*)malloc(strlen(name)+1);
    strcpy(var_decl_node->content.var_decl.name, name);
    var_decl_node->content.var_decl.val_type = val_type;
    var_decl_node->value_type = NULL;
    return var_decl_node;
}


AstNode *ast_if(AstNode *cond, AstNode *then, AstNode *els)
{
    AstNode *node = (AstNode*)malloc(sizeof(AstNode));
    node->type =antIf;
    node->content.if_.cond = cond;
    node->content.if_.then = then;
    node->content.if_.els = els;
    node->value_type = NULL;
    return node;
}

