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

AstNode *parse_stmt()
{
    if (g_token.type == ttEOF)
        return 0;

    if (g_token.type == ttIdentifier)
    {
        ValueType *val_type = NULL;
        char *identifier = get_token_repr();
        next_token();
        
        val_type = type_map_get(identifier);
        // Variable declaration.
        if (val_type != NULL)
        {
            AstNode *var_decl;

            if (g_token.type != ttIdentifier)
                error("Identifier expected");

            var_decl = (AstNode*)malloc(sizeof(AstNode));
            var_decl->type = antVarDecl;
            var_decl->content.var_decl.name = get_token_repr();
            
            next_token();
            if (g_token.type != ttSemilcon)
                parse_error("; expected.");
            next_token();
            
            free(identifier);
            return var_decl;
        }
        
        // Assignment.
        if (g_token.type == ttAssign)
        {
            next_token();
            AstNode *assign_node = NULL;
            Assign *assign = NULL;
            
            assign_node = (AstNode*)malloc(sizeof(AstNode));
            assign_node->type = antAssign;
            assign = &assign_node->content.assign;

            assign->name  = identifier;
            assign->expr = parse_expr();
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
                get_func(identifier, &func_call->params);
            
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
        term = (AstNode*)malloc(sizeof(AstNode));
        term->content.var_val.name = get_token_repr();
        term->type = antVarVal;
        term->value_type = IntType;
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
        mul = ast_mul(term1, term2);
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
        add = ast_add(term1, term2);
        return add;
    }
    else
    {
        return term1;
    }
}

AstNode *parse_expr()
{
    AstNode *expr = NULL;
    expr = parse_add();
    return expr;
}

//-----------------------------------------------------------------------------

void parse()
{
    list stmt_list;
    AstNode *stmt = NULL;

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
}


