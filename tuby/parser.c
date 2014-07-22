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
 
/* Root node of the syntax tree. */
AstNode g_root;

ValueType *parse_valuetype(ValueType *underlying)
{
    if (g_token.type == ttOpenSquare)
    {
        next_token();
        if (g_token.type != ttCloseSquare)
            parse_error("] expected");
        next_token();
        return typemap_get_array(underlying);
    }
    else
    {
        return underlying;
    }
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


AstNode *parse_stmt()
{
    if (g_token.type == ttEOF)
        return 0;
    
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
       
        // Variable declaration.
        val_type = type_map_get(identifier);
        if (val_type != NULL)
        {
            val_type = parse_valuetype(val_type);
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
            if (g_token.type == ttAssign)
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
            assign = ast_assign(var, unary_node);
            return assign;
        }

        // Assignment.
        if (g_token.type == ttAssign)
        {
            AstNode *assign_node = NULL;
            Assign *assign = NULL;
            Var *var = NULL;

            var = varmap_get(identifier);
            if (var == NULL)
                parse_error("Variable %s not defined yet", identifier);

            next_token();
            assign_node = (AstNode*)malloc(sizeof(AstNode));
            assign_node->type = antAssign;
            assign_node->value_type = NULL;
            assign = &assign_node->content.assign;

            assign->name  = identifier;
            assign->expr = parse_expr();

            if (assign->expr->value_type != var->val_type)
            {
                parse_error("Could not assign expression of type %s to "
                            "variable %s of type %s", 
                             assign->expr->value_type->name, identifier,
                             var->val_type->name);
            }

            if (g_token.type != ttSemilcon)
                parse_error("; expected.");
            next_token();
            return assign_node;
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
    else  if (g_token.type == ttNumber)
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
    else if (g_token.type == ttIdentifier)
    {
        char *identifier = get_token_repr();
        Var *var = varmap_get(identifier);

        if (var == NULL)
            parse_error("Variable %s is not defined yet.", identifier);


        term = (AstNode*)malloc(sizeof(AstNode));
        term->content.var_val.name = identifier;
        term->type = antVarVal;
        term->value_type = var->val_type;
        next_token();
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

AstNode *parse_add()
{
    AstNode *term1 = parse_mul();
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
    while (stmt != NULL)
    {
        list_push(&stmt_list, stmt); 
        stmt = parse_stmt();
    }

    g_root.type = antStmtList;
    g_root.content.stmt_list = stmt_list;
    varmap_purge();
}


