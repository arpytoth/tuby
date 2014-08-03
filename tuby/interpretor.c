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
#include <stdarg.h>

#include "interpretor.h"
#include "parser.h"
#include "utils.h"
#include "stack.h"
#include "var_map.h"
#include "list.h"
#include "allocator.h"
#include "type_map.h"
#include "tuby_array.h"
#include "tuby_char.h"


void interpret_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Runtime Error: ");
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}


Value *interpret_func_var_val(AstNode *node)
{
    Value *value = stack_function_param(0);
    return value;
}


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
    else if (node->type == antFuncVarVal)
    {
        return interpret_func_var_val(node);
    }
    else if (node->type == antCharVal)
    {
        Value *val = NULL;
        val = alloc_val(CharType);
        val->data.char_val = node->content.char_val;
        return val;
    }
    else if (node->type == antStrVal)
    {
        Value *val = NULL;
        val = alloc_val(StrType);
        string_dup(&val->data.str_val, &node->content.str_val);
        return val;
    }
    else if (node->type == antBoolVal)
    {
        Value *val = NULL;
        val = alloc_val(BoolType);
        val->data.bool_val = node->content.bool_val;
        return val;
    }
    else if (node->type == antIntVal)
    {
        Value *val = NULL;
        val = alloc_val(IntType);
        val->data.int_val = node->content.int_val.value;
        return val;
    }
    else if (node->type == antIndexAccess)
    {
        IndexAccess *ia = &node->content.index_access;
        AstNode *index = ia->index;
        AstNode *val = ia->val;
        
        Value *under_val = eval(val);
        Value *index_val = eval(index);
        
        if (under_val->value_type == StrType)
        {
            int int_index;
            Value *char_at_index = NULL;
            struct String *str_val = NULL;

            if (index_val->value_type != IntType)
                interpret_error("String index must be of type int.");

            int_index = index_val->data.int_val;
            str_val = &under_val->data.str_val;
            char_at_index = alloc_val(CharType);
            char_at_index->data.char_val = string_at(str_val, int_index);
            
            alloc_free_val(index_val);
            alloc_free_val(under_val);
            return char_at_index;
        }
        else
        {
            int int_index = index_val->data.int_val;
            Value *ret_val = alloc_array_get(under_val, int_index);
            alloc_free_val(index_val);
            alloc_free_val(under_val);
            return alloc_use_val(ret_val);
        }
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
        return alloc_use_val(var->val);
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
        int i = 0;
        StackFrame *frame = NULL; 
        vector params;
         
        vector_init(&params);
        for (i = 0; i < vector_length(&fc->params); i++)
        {
            AstNode *param = (AstNode*)vector_at(&fc->params, i);
            vector_push(&params, eval(param)); 
        }
        frame = stack_push();
        frame->params = params;
        if (fc->func->native != NULL)
        {
            fc->func->native();
        }
        else
        {
            interpret_node(fc->func->body);
        }
        for (i = 0; i < vector_length(&frame->params); i++)
        {
            Value *val = (Value*)vector_at(&frame->params, i);
            alloc_free_val(val);
        }
        stack_pop();
    }
    else if (node->type == antVarDecl)
    {
        VarDecl *var_decl = &node->content.var_decl;
        Var *var =varmap_def(var_decl->name, var_decl->val_type);

        if (var_decl->init != NULL)
        {
            Value *init_val = eval(var_decl->init);
            var->val->data = init_val->data;
            var->val->is_null = init_val->is_null;
            alloc_free_val(init_val); 
        }
    }
    else if (node->type == antAssign)
    {
        Assign *assign = &node->content.assign;
     
        /*
         * The string index access is a special case because the string is
         * not implemented as an array but a a direct char[] C style. This
         * is because that way is more efficient.
         */
        if (assign->lvalue->type == antIndexAccess &&
            assign->lvalue->content.index_access.val->value_type == StrType)
        {
            struct String *str = NULL;
            Value *str_val = NULL;
            Value *rvalue = NULL;
            Value *index = NULL;
            int int_index;

            str_val = eval(assign->lvalue->content.index_access.val);
            str = &str_val->data.str_val;
            index = eval(assign->lvalue->content.index_access.index);
            
            if (index->value_type != IntType)
                interpret_error("String index must be of type int");

            int_index = index->data.int_val;
            rvalue = eval(assign->expr);

            if (rvalue->value_type != CharType)
                interpret_error("Only char can be assigned to string[]");

            str->buffer[int_index] = rvalue->data.char_val;
            alloc_free_val(str_val);
            alloc_free_val(index);
            alloc_free_val(rvalue);
        }
        else
        {
            Value *lvalue = eval(assign->lvalue);
            Value *rvalue = eval(assign->expr);
            if (lvalue->value_type == StrType)
            {
                string_dup(&lvalue->data.str_val, &rvalue->data.str_val);
                lvalue->is_null = rvalue->is_null;
            }
            else if (lvalue->value_type->is_array)
            {
                array_copy(&lvalue->data.array_val, &rvalue->data.array_val);
                lvalue->is_null = rvalue->is_null;
            }
            else
            {
                lvalue->data = rvalue->data;
                lvalue->is_null = rvalue->is_null;
            }
            alloc_free_val(lvalue);
            alloc_free_val(rvalue);
        }
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


