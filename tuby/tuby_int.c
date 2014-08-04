#include <stdlib.h>
#include <string.h>

#include "tuby_int.h"
#include "func_table.h"
#include "stack.h"

void int_assign_operator()
{
    Value *dest = stack_function_param(0);
    Value *src = stack_function_param(1);
    int int_val2 = src->data.int_val;
    dest->data.int_val = int_val2;   
    stack_set_ret_val(NULL); 
}


void int_init_module()
{
    FuncDef *func;

    // int = int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("=");
    func->native = int_assign_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, IntType);
    vector_push(func->params, IntType);
    func_def(func);
}
