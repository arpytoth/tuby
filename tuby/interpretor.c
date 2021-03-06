#include <stdlib.h>
#include "interpretor.h"
#include "parser.h"
#include "utils.h"
#include "stack.h"
#include "var_map.h"

//------------------------------------------------------------------------------

Value *eval(AstNode *node)
{
    if (node->type == antIntVal)
    {
        Value *val = (Value*)malloc(sizeof(Value));
        val->data.int_val = node->content.int_val.value;
        return val;
    }
    else if (node->type == antVarVal)
    {
        VarVal *var_val = &node->content.var_val;
        Var *var = varmap_get(var_val->name);
        if (var == NULL)
            error("Variable not defined yet");
        
        return var->val;
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
    if (node->type == antStmtList)
    {
        struct AstNodeList *list = &node->content.stmt_list;
        struct AstNodeListEntry *e = list->first;
        while (e != 0)
        {
            interpret_node(e->stmt);
            e = e->next;
        }
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
            stack_pop();
        }
    }
    else if (node->type == antVarDecl)
    {
        VarDecl *var_decl = &node->content.var_decl;
        varmap_def(var_decl->name);
    }
    else if (node->type == antAssign)
    {
        Assign *assign = &node->content.assign;
        Var *var = varmap_get(assign->name);
        if (var == NULL)
            error("Variable %s not defined", assign->name);

        var->val = eval(assign->expr);
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


