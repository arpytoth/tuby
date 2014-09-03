#include <stdlib.h>
#include <string.h>

#include "bool_type.h"
#include "func_table.h"
#include "stack.h"
#include "allocator.h"

////////////////////////////////////////////////////////////////////////////////
//                              OPERATORS                                     //
////////////////////////////////////////////////////////////////////////////////                  

void or_operator()
{
    Value *a = stack_function_param(0);
    Value *b = stack_function_param(1);
    Value *r = NULL;

    int val1 = a->data.bool_val;
    int val2 = b->data.bool_val; 
    
    r = alloc_val(BoolType);
    r->data.bool_val = val1 || val2;

    stack_set_ret_val(r); 
}


void bool_init_module()
{
    FuncDef *func;

    // bool || bool;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("||");
    func->native = or_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = BoolType;
    vector_init(func->params);
    vector_push(func->params, new_param_info(BoolType, 1));
    vector_push(func->params, new_param_info(BoolType, 0));
    func_def(func);
}
