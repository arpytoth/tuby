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
#include "interpretor.h"
#include "parser.h"
#include "utils.h"
#include "stack.h"
#include "var_map.h"
#include "list.h"
#include "allocator.h"
#include "type_map.h"
#include "array.h"


Value *eval(AstNode *node)
{
    
    if (node->type == antFuncCall)
    {
        FuncCall *fc = &node->content.func_call;
        if (fc->func->native != 0)
        {
            int i = 0;
            StackFrame *frame = stack_push();
            vector_init(&frame->params);
            for (i = 0; i < vector_length(&fc->params); i++)
            {
                AstNode *param = (AstNode*)vector_at(&fc->params, i);
                vector_push(&frame->params, eval(param)); 
            }
            fc->func->native();
            for (i = 0; i < vector_length(&frame->params); i++)
            {
                Value *val = (Value*)vector_at(&frame->params, i);
                alloc_free_val(val);
            }
            stack_pop();
            return frame->ret_val;
        }
        error("Function Definition Not Found.. Only Native supported!!!");
        return 0;

    }
    else if (node->type == antBoolVal)
    {
        Value *val = NULLn::;
        val = alloc_get_val(val);
        val->data.bool_val = node->content.bool_val;
        val->value_type = BoolType;
        return val;
    }
    else if (node->type == antIntVal)
    {
        Value *val = NULL;
        val = alloc_get_val(val);
        val->value_type = IntType;
        val->data.int_val = node->content.int_val.value;
        return val;
    }
    else if (node->type == antIndexAccess)
    {
        IndexAccess *ia = &node->content.index_access;
        AstNode *index = ia->index;
        AstNode *val = ia->val;
        Value *index_val = eval(index);
        int int_index = index_val->data.int_val;
        Value *array_val = eval(val);
        Value *ret_val = array_get_val(array_val, int_index);
        alloc_free_val(index_val);
        alloc_free_val(array_val);
        return alloc_get_val(ret_val);
    }
    else if (node->type == antVarVal)
    {
        VarVal *var_val = &node->content.var_val;
        Var *var = varmap_get(var_val->name);
        if (var == NULL)
            error("Variable %s not defined yet, var_val->name");
        if (var->val == NULL) 
        {
            var->val = alloc_val(var->val_type);
            var->val->value_type = var->val_type;
        }
        return alloc_get_val(var->val);
    }
    else
    {
        error("Fatal Error: invalid expression node");
        return NULL;
    }
}

//------------------------------------------------------------------------------

void interpret_node(AstNode *node)
{
    if (node == NULL)
        error("Fatal ERROR!! NULL NODE");

    if (node->type == antStmtList)
    {
        varmap_push();
        list *stmt_list = &node->content.stmt_list;
        list_elem *e = stmt_list->first;
        while (e != NULL)
        {
            AstNode *stmt = (AstNode*)e->data;
            interpret_node(stmt);
            e = e->next;
        }
        varmap_purge();
    }
    else if (node->type == antIf)
    {
        If *i = &node->content.if_;
        Value *val = eval(i->cond);
        if (val->data.bool_val != 0)
        {
            if (i->then != NULL)
                interpret_node(i->then);
        }
        else
        {
            if (i->els != NULL)
                interpret_node(i->els);
        }
        alloc_free_val(val);
    }
    else if (node->type == antFor)
    {
        varmap_push();
        For *f = &node->content.for_;
        Value *val = NULL;

        interpret_node(f->init);
        val = eval(f->cond);
        while(val->data.bool_val != 0)
        {
            interpret_node(f->body);
            interpret_node(f->inc);
            alloc_free_val(val);
            val = eval(f->cond);
        }
        alloc_free_val(val);
        varmap_purge();
    }
    else if (node->type == antWhile)
    {
        While *i = &node->content.while_;
        Value *val = eval(i->cond);
        while (val->data.bool_val != 0)
        {
            interpret_node(i->body);
            alloc_free_val(val);
            val = eval(i->cond);
        }
        alloc_free_val(val);
    }
    else if (node->type == antFuncCall)
    {
        FuncCall *fc = &node->content.func_call;
        if (fc->func->native != 0)
        {
            int i = 0;
            StackFrame *frame = stack_push();
            vector_init(&frame->params);
            for (i = 0; i < vector_length(&fc->params); i++)
            {
                AstNode *param = (AstNode*)vector_at(&fc->params, i);
                vector_push(&frame->params, eval(param)); 
            }
            fc->func->native();
            for (i = 0; i < vector_length(&frame->params); i++)
            {
                Value *val = (Value*)vector_at(&frame->params, i);
                alloc_free_val(val);
            }
            stack_pop();
        }
    }
    else if (node->type == antVarDecl)
    {
        VarDecl *var_decl = &node->content.var_decl;
        Var *var =varmap_def(var_decl->name, var_decl->val_type);
        if (var_decl->init != NULL)
        {
            var->val = eval(var_decl->init);
        }
    }
    else if (node->type == antAssign)
    {
        Assign *assign = &node->content.assign;
        Value *lvalue = eval(assign->lvalue);
        Value *rvalue = eval(assign->expr);
        lvalue->data = rvalue->data;
        alloc_free_val(lvalue);
        alloc_free_val(rvalue);
    }
    else
    {
        error("Fatal Error. Invalid statement.");
    }
}

//-----------------------------------------------------------------------------

void interpret()
{
    interpret_node(&g_root);
}

//-----------------------------------------------------------------------------


