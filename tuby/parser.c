#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "parser.h"
#include "func_table.h"
#include "tokenizer.h"
#include "utils.h"
#include "vector.h"
#include "var_map.h"

/* Root node of the syntax tree.*/
AstNode g_root;

AstNode *parse_stmt()
{
    if (g_token.type == ttEOF)
        return 0;

    if (g_token.type == ttIdentifier)
    {
        char *identifier = get_token_repr();
        next_token();
        
        // Variable declaration.
        if (strcmp(identifier, "int") == 0)
        {
            AstNode *var_decl;

            if (g_token.type != ttIdentifier)
                error("Identifier expected");

            var_decl = (AstNode*)malloc(sizeof(AstNode));
            var_decl->type = antVarDecl;
            var_decl->content.var_decl.name = identifier;
            next_token();
            if (g_token.type != ttSemilcon)
                parse_error("; expected.");
            next_token();
            
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

//-----------------------------------------------------------------------------

AstNode *parse_expr()
{
    if (g_token.type == ttNumber)
    {
        AstNode *int_val;
        int_val = (AstNode*)malloc(sizeof(AstNode));
        int_val->content.int_val.value = atoi(g_token.repr);
        int_val->type = antIntVal;
        int_val->value_type = &IntType;
        next_token();
        return int_val;
    }
    return 0;
}

//-----------------------------------------------------------------------------

void parse()
{
    struct AstNodeList stmt_list;
    AstNode *stmt = 0;

    next_token();
    stmt_list.first = 0;
    stmt_list.last = 0;
    stmt = parse_stmt();
    while (stmt != 0)
    {
        add_node(&stmt_list, stmt); 
        stmt = parse_stmt();
    }

    g_root.type = antStmtList;
    g_root.content.stmt_list = stmt_list;
}


