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
#include "parser.h"
#include "func_table.h"
#include "tokenizer.h"
#include "utils.h"
#include "vector.h"
#include "var_map.h"
#include "type_map.h"
 
AstNode g_root;
vector *func_params;

///////////////////////////////////////////////////////////////////////////////
// Parse Function Parameters
///////////////////////////////////////////////////////////////////////////////

void func_params_init()
{
    func_params = (vector*)malloc(sizeof(vector));
    vector_init(func_params);
}

void func_params_free()
{
    int i;
    for (i = 0; i < vector_length(func_params); i++)
    {
        struct Var *var;
        var  = (struct Var*)vector_at(func_params, i);
        varmap_free_var(var);
    }
    vector_release(func_params);
    func_params = NULL;
}

void func_params_add(Var *var)
{
    vector_push(func_params, var);
}


int  func_params_search(const char *name)
{
    if (func_params != NULL)
    {
        int i;
        for (i = 0; i < vector_length(func_params); i++)
        {
            struct Var *var;
            var  = (struct Var*)vector_at(func_params, i);
            if (strcmp(var->name, name) == 0)
                return i;
        }
    }
    return -1;
}


/*
 * This will search for the var with the specified name, first inf
 * function params, and then in varmap. If var not found NULL is
 * returned, if var found this will return the AstNode needed to
 * access the value of that specified var.
 */
AstNode *parse_var(const char *name)
{
    int i;
    i = func_params_search(name);
    if (i >= 0)
    {
        AstNode *node;
        Var *var;

        var = (Var*)vector_at(func_params, i);
        node = ast_func_var_val(i, var->val_type);
        return node;
    }
    else 
    {
        Var *var;
        var = varmap_get(name);
        if (var != NULL)
            return ast_varval(var);
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Parse Functions
///////////////////////////////////////////////////////////////////////////////
ValueType *parse_value_type(ValueType *underlying)
{
    if (g_token.type == ttOpenSquare)
    {
        ValueType *type = NULL;

        next_token();
        if (g_token.type != ttCloseSquare)
            parse_error("] expected");
        next_token();
        type = typemap_get_array(underlying);
        
        if (g_token.type == ttOpenSquare)
            return parse_value_type(type);
        else
            return type;
    }
    else
    {
        return underlying;
    }
}


AstNode *parse_index_access(AstNode *var_val)
{
    
    /*
     * When this function is called we already parsed the variable
     * and we reached a '[' token.
     */
    AstNode *index_access = NULL;
    AstNode *index = NULL;

    next_token();
    index = parse_expr();
    if (index == NULL)
        parse_error("Index or key expected");

    if (index->value_type != IntType)
        parse_error("Only index of type int accepted for now");

    if (g_token.type != ttCloseSquare)
        parse_error("] expected");
    next_token();

    index_access = ast_index_access(var_val, index); 

    if (g_token.type == ttOpenSquare)
        return parse_index_access(index_access);
    else
        return index_access;
}

AstNode *parse_for()
{
    varmap_push();
    AstNode *body = NULL;
    AstNode *cond = NULL;
    AstNode *incr = NULL;
    AstNode *init = NULL;

    next_token();
    if (g_token.type != ttOpenBracket)
        parse_error("( expected.");
    next_token();

    init = parse_stmt();
    if (init == NULL)
        parse_error("Expected initializer statement. For example "
                     "for (int i = 1 ....");

    cond = parse_expr();
    if (cond == NULL)
        parse_error("Expected a stop condition");

    if (cond->value_type != BoolType)
        parse_error("Stop condition must be of type boolean");

    if (g_token.type != ttSemilcon)
        parse_error("; expected");
    next_token();

    incr = parse_stmt();
    if (incr == NULL)
        parse_error("Expected an incrementor statement");

    if (g_token.type != ttCloseBracket)
        parse_error(") expected");
    next_token();

    body = parse_stmt();
    if (body == NULL)
        parse_error("For loop must have a body.");

    varmap_purge();
    return ast_for(init, cond, incr, body);
}

AstNode *parse_assign(AstNode *lvalue)
{
    AstNode *rvalue = NULL;
    
    rvalue = parse_expr();
    if (rvalue == NULL)
        parse_error("Assign value expected.");

    if (rvalue->value_type != lvalue->value_type)
    {
        parse_error("Could not assign expression of type %s to "
                    "type %s", rvalue->value_type->name, 
                    lvalue->value_type->name);
    }

    if (g_token.type != ttSemilcon)
        parse_error("; expected.");
    next_token();
    return ast_assign(lvalue, rvalue);

}


/*
 * Parse a function definition code and add it to the 
 * function table. So this method won't generate an actual
 * statement. Yes the function declaration is threated as a
 * statement but it is executed at compile time and not at
 * runtime.
 * When we call this function we assumen that type indentifier
 * were readed and the next token is (.
 *
 * So basically we are in the : int main( state, we need to read
 * the list of parameters and the statement list. 
 */
void parse_function_def(const char *name)
{
    AstNode *body = NULL;
    FuncDef *func = NULL;
    list stmt_list;
    AstNode *stmt = NULL;
    
    next_token();
    while (g_token.type != ttCloseBracket)
    {
        ValueType *value_type = NULL;

        next_token();
        value_type = type_map_get(g_token.repr);
        if (value_type == NULL)
            parse_error("Type name expected");

        next_token();
        if (g_token.type != ttIdentifier)
            parse_error("Identifier expected");

        next_token();
        if (g_token.type != ttComma)
        {
            next_token();
        }
        else if (g_token.type == ttCloseBracket)
        {
            break;
        }
        else
        {
            parse_error(", or ) expected");
        }
    }
    
    next_token();
    if (g_token.type != ttOpenCurly)
        parse_error("Expected function body between {}");
    next_token();
    varmap_push();
    list_init(&stmt_list);
    stmt = parse_stmt();
    while (1)
    {
        list_push(&stmt_list, stmt); 
        if (g_token.type == ttEOF)
        {
            parse_error("End of file reached but } not found.");
        }
        else if (g_token.type == ttCloseCurly)
        {
            next_token();
            break;
        }
        
        stmt = parse_stmt();
    }

    func = (FuncDef*)malloc(sizeof(FuncDef));
    body = (AstNode*)malloc(sizeof(AstNode));
    
    body->type = antStmtList;
    body->content.stmt_list = stmt_list;
    func->name = strdup(name);
    func->native = NULL;
    func->value_type = NULL;
    func->params = (vector*)malloc(sizeof(vector));
    vector_init(func->params);
    func->body = body; 
    func_def(func);
    varmap_purge();
}


AstNode *parse_stmt()
{
    if (g_token.type == ttEOF)
        return NULL;
    
    if (g_token.type == ttOpenCurly)
    {
        varmap_push();
        list stmt_list;
        AstNode *stmt_list_node = NULL; 
        AstNode *stmt = NULL;
        next_token();
        list_init(&stmt_list);
        stmt = parse_stmt();
        while (stmt != NULL)
        {
            list_push(&stmt_list, stmt); 
            stmt = parse_stmt();
        }
       
        if (g_token.type != ttCloseCurly)
            parse_error("} expected");
        next_token();
        stmt_list_node = (AstNode*)malloc(sizeof(AstNode));
        stmt_list_node->type = antStmtList;
        stmt_list_node->content.stmt_list = stmt_list;
        varmap_purge();
        return stmt_list_node; 
    }

    if (g_token.type == ttFor)
        return parse_for();

    if (g_token.type == ttWhile)
    {
        AstNode *cond = NULL;
        AstNode *body = NULL;

        next_token();
        if (g_token.type != ttOpenBracket)
            parse_error("( expected");
        next_token();     
        cond = parse_expr();
        if (cond->value_type != BoolType)
            parse_error("Boolean expression expected.");
        
        if (g_token.type != ttCloseBracket)
            parse_error(") expected");
        next_token();    
        body = parse_stmt();
        if (body == NULL)
            parse_error("Error, while body expected.");
        return ast_while(cond, body);    		
    }
 
    if (g_token.type == ttIF)
    {
        AstNode *cond = NULL;
        AstNode *then = NULL;
        AstNode *els = NULL;

        next_token();
        if (g_token.type != ttOpenBracket)
            parse_error("( expected");
        next_token();     
        cond = parse_expr();
        if (cond->value_type != BoolType)
            parse_error("Boolean expression expected.");
        
        if (g_token.type != ttCloseBracket)
            parse_error(") expected");
        next_token();    
        then = parse_stmt();
        if (then == NULL)
            parse_error("Error, then code expected.");
        if (g_token.type == ttElse)
        {
            next_token();
            els = parse_stmt();
        }
        return ast_if(cond, then, els);    		
    }
    else if (g_token.type == ttIdentifier)
    {
        ValueType *val_type = NULL;
        char *identifier = get_token_repr();
        
        next_token();
       
        /*
         * If it starts with a type name, it is clearly a declaration:
         * variable or function.
         */
        val_type = type_map_get(identifier);
        if (val_type != NULL)
        {
            AstNode *var_decl = NULL;
            AstNode *init = NULL;
            char *varname = NULL;

            if (g_token.type != ttIdentifier)
            {
                free(identifier);
                parse_error("Identifier expected");
            }

            varname = get_token_repr();
            next_token();
            val_type = parse_value_type(val_type);
            
            if (g_token.type == ttOpenBracket)
            {
                parse_function_def(varname); 
                return NULL;
            }
            else if (g_token.type == ttAssign)
            {
                next_token();
                init = parse_expr();
                if (init->value_type != val_type)
                {
                    free(identifier);
                    free(varname);
                    parse_error("Could not assign expression of type %s to "
                                "variable %s of type %s", 
                                 init->value_type->name, identifier,
                                 var_decl->value_type->name);
                }
            }
            var_decl = ast_var_decl(varname, val_type, init); 
            varmap_def(varname, val_type);

            if (g_token.type != ttSemilcon)
            {
                free(varname);
                free(identifier);
                parse_error("; expected.");
            }
            next_token();
            free(identifier);
            free(varname);
            return var_decl;
        }

        /*
         * It is an identifier with array mark. Well this probably
         * means index access left value.
         */
        if (g_token.type == ttOpenSquare)
        {
            Var *var = NULL;
            AstNode *var_val = NULL;
            AstNode *index_access = NULL;
            
            var = varmap_get(identifier);
            if (var == NULL)
                parse_error("Variable %s not defined yet", identifier);
            
            var_val = ast_varval(var);
            index_access = parse_index_access(var_val);

            if (g_token.type != ttAssign)
                parse_error("= expected. ");
             
            next_token(); 
            return parse_assign(index_access);
        }

        // Unary Operator
        if (g_token.type == ttInc)
        {
            /*
             * Note that if unary operator is called as statement
             * we will actually do an assigment too. For example
             * i++ means i = i + 1;
             */
            AstNode *assign = NULL;
            AstNode *unary_node = NULL;
            AstNode *varval = NULL;
            Var *var = NULL;
            var = varmap_get(identifier);
            if (var == NULL)
                parse_error("Variable %s not defined yet", identifier);

            varval = ast_varval(var);
            unary_node = ast_unary("++", varval);
            next_token();
            if (g_token.type != ttSemilcon)
                parse_error("; expected");
            next_token();
            assign = ast_assign(varval, unary_node);
            return assign;
        }

        // Simple Assignment
        if (g_token.type == ttAssign)
        {
            Var *var = NULL;
            AstNode *lvalue = NULL;
            
            var = varmap_get(identifier);
            if (var == NULL)
                parse_error("Variable %s not defined yet", identifier);
            next_token();
            lvalue = ast_varval(var);
            return parse_assign(lvalue);
        }

        // Function Call
        if (g_token.type == ttOpenBracket)
        {
            AstNode *func_call_node = NULL;
            FuncCall *func_call = NULL;

            func_call_node = (AstNode*)malloc(sizeof(AstNode));
            func_call_node->type = antFuncCall;
            func_call = &func_call_node->content.func_call;
            
            next_token(); 
            vector_init(&func_call->params);

            while (g_token.type != ttCloseBracket)
            {
                AstNode *param = parse_expr();
                vector_push(&func_call->params, param);
            }

            next_token(); 
            if (g_token.type != ttSemilcon)
                parse_error("; expected.");
            next_token();

            func_call_node->content.func_call.func = 
                func_get(identifier, &func_call->params);
            
            if (func_call_node->content.func_call.func == NULL)
                parse_error("Function %s not defined\n", identifier);
            free(identifier);
            return func_call_node;
        }
        else
        {
            free(identifier);
            parse_error("( expected");
        }
    }
    
    // Error !!! Not good at all.
    return 0;
}


AstNode *parse_term()
{
    AstNode *term = NULL;
  
    if (g_token.type == ttOpenBracket)
    {
        next_token();
        term = parse_expr();
        if (g_token.type != ttCloseBracket)
            error("Missing ) ");
        next_token();
    }
    else if (g_token.type == ttChar)
    {
        term = ast_char_val(g_token.repr[0]); 
        next_token();
    }
    else if (g_token.type == ttNumber)
    {
        term = (AstNode*)malloc(sizeof(AstNode));
        term->content.int_val.value = atoi(g_token.repr);
        term->type = antIntVal;
        term->value_type = IntType;
        next_token();
    }
    else if (g_token.type == ttTrue || g_token.type == ttFalse)
    {
        int val;
        if (g_token.type == ttTrue)
            val = 1;
        else
            val = 0;

        term = ast_bool_val(val);
        next_token();
    }
    else if (g_token.type == ttString)
    {
        AstNode *str_val = ast_str_val(g_token.repr);
        next_token();
        return str_val;
    }
    else if (g_token.type == ttIdentifier)
    {
        char *identifier = get_token_repr();
        Var *var = varmap_get(identifier);

        if (var == NULL)
            parse_error("Variable %s is not defined yet.", identifier);
    
        next_token();
        if (g_token.type == ttOpenSquare)
        {
            AstNode *var_val = NULL;
            AstNode *index_access = NULL;

            var_val = ast_varval(var);
            index_access = parse_index_access(var_val);

            term = index_access;
        }
        else
        {
            term = (AstNode*)malloc(sizeof(AstNode));
            term->content.var_val.name = identifier;
            term->type = antVarVal;
            term->value_type = var->val_type;
        }
    }
    else
    {
        error("Expression expected.");
    }
    return term;
}


AstNode *parse_mul()
{
    AstNode *term1 = parse_term();
    if (g_token.type == ttMul)
    {
        AstNode *term2 = NULL;
        AstNode *mul = NULL;

        next_token();
        term2 = parse_mul();
        mul = ast_binary("*", term1, term2);
        return mul;
    }
    else
    {
        return term1;
    }
}


AstNode *parse_sub()
{
    AstNode *term1 = parse_mul();
    if (g_token.type == ttSub)
    {
        AstNode *term2 = NULL;
        AstNode *sub = NULL;
        next_token();
        term2 = parse_sub();
        sub = ast_binary("-", term1, term2);
        return sub;
    }
    else
    {
        return term1;
    }
}


AstNode *parse_add()
{
    AstNode *term1 = parse_sub();
    if (g_token.type == ttAdd)
    {
        AstNode *term2 = NULL;
        AstNode *add = NULL;
        next_token();
        term2 = parse_add();
        add = ast_binary("+", term1, term2);
        return add;
    }
    else
    {
        return term1;
    }
}

AstNode *parse_not_equals()
{
    AstNode *term1 = parse_add();
    if (g_token.type == ttNotEquals)
    {
        AstNode *term2 = NULL;
        AstNode *equals = NULL;
        next_token();
        term2 = parse_not_equals();
        equals = ast_binary("!=", term1, term2);
        return equals;
    }
    else
    {
        return term1;
    }
}

AstNode *parse_equals()
{
    AstNode *term1 = parse_not_equals();
    if (g_token.type == ttEquals)
    {
        AstNode *term2 = NULL;
        AstNode *equals = NULL;
        next_token();
        term2 = parse_equals();
        equals = ast_binary("==", term1, term2);
        return equals;
    }
    else
    {
        return term1;
    }

}

AstNode *parse_expr()
{
    AstNode *expr = NULL;
    expr = parse_equals();
    return expr;
}

//-----------------------------------------------------------------------------

void parse()
{
    list stmt_list;
    AstNode *stmt = NULL;

    varmap_push();
    next_token();
    
    list_init(&stmt_list);
    stmt = parse_stmt();
    while (g_token.type != ttEOF || stmt != NULL)
    {
        if (stmt != NULL)
            list_push(&stmt_list, stmt); 
        stmt = parse_stmt();
    }
    g_root.type = antStmtList;
    g_root.content.stmt_list = stmt_list;
    varmap_purge();
}
