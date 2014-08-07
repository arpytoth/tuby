#include <stdlib.h>
#include <string.h>

#include "func_table.h"
#include "tuby_array.h"
#include "vector.h"
#include "allocator.h"
#include "stack.h"


void array_square_operator()
{
    struct Value *under_val = stack_function_param(0);
    struct Value *index_val = stack_function_param(1);

    int int_index = index_val->data.int_val;
    Value *ret_val = alloc_array_get(under_val, int_index);
    alloc_use_val(ret_val);
    stack_set_ret_val(ret_val); 
}

void array_assign_operator()
{
    struct Value *lvalue= stack_function_param(0);
    struct Value *rvalue= stack_function_param(1);
    
    array_copy(&lvalue->data.array_val, &rvalue->data.array_val);
    lvalue->is_null = rvalue->is_null;
    stack_set_ret_val(NULL); 
}

void array_type_init(struct ValueType *type)
{
    FuncDef *func;
  
    // array = array 
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("=");
    func->native = array_assign_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, type);
    vector_push(func->params, type);
    func_def(func);

    // array[int]
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("[]");
    func->native = array_square_operator;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = type->uval_type;
    vector_init(func->params);
    vector_push(func->params, type);
    vector_push(func->params, IntType);
    func_def(func);
}

void array_init(struct Array *arr)
{
    arr->data = (vector*)malloc(sizeof(vector));
    vector_init(arr->data);
    arr->data->ref_count = 1;
}


void array_free(struct Array *arr)
{
    if (arr != NULL && arr->data != NULL)
    {
        arr->data->ref_count--;
        if (arr->data->ref_count == 0)
        {
            int i;
            int length = vector_length(arr->data);
            for (i = 0; i < length; i++)
            {
                Value *elem = (Value*)vector_at(arr->data, i);
                if (elem != NULL)
                    alloc_free_val(elem);
            }
            free(arr->data);
            arr->data = NULL;
        }              
    }
}

void array_use(struct Array *arr)
{
    arr->data->ref_count++;
}


void array_copy(struct Array *dest, struct Array *src)
{
    array_free(dest);
    array_use(src);
    dest->data = src->data;
}
