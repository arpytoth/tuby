#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "type_map.h"
#include "func_table.h"
#include "stack.h"
#include "allocator.h"

/* An entry in the function table. */
struct FuncTableEntry
{
    FuncDef *function; /* Function implementation. */
    struct FuncTableEntry *next; /* Next element in the function table. */
};

/* The function table data. For not the function table is a linked list.*/
struct FuncTable
{
    struct FuncTableEntry *first;
    struct FuncTableEntry *last;
};

/* Global function table, the place where all function will be stored.*/
struct FuncTable g_func_table;
FuncDef g_tuby_func;

void tuby()
{
    printf("Tuby!!!");
}

FuncDef g_tuby_print_int;
void print()
{
    Value *param1 = stack_function_param(0);
    if (param1->value_type == IntType)
    {
        int int_val = param1->data.int_val;
        printf("%d\n", int_val);
    }    
    else if (param1->value_type == BoolType)
    {
        int bool_val = param1->data.bool_val;
        if (bool_val == 0)
            printf("false\n");
        else
            printf("true\n");
    }
}


void add_int()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_get_val(NULL);
    ret_val->data.int_val = int_val1 + int_val2;
    ret_val->value_type = IntType;
    stack_set_ret_val(ret_val); 
}


void equals_int()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_get_val(NULL);
    ret_val->data.bool_val = int_val1 == int_val2;
    ret_val->value_type = BoolType;
    stack_set_ret_val(ret_val); 

}


void mul_int()
{
    Value *param1 = stack_function_param(0);
    Value *param2 = stack_function_param(1);
    int int_val1 = param1->data.int_val;
    int int_val2 = param2->data.int_val;
    Value *ret_val = alloc_get_val(NULL);
    ret_val->data.int_val = int_val1 * int_val2;
    ret_val->value_type = IntType;
    stack_set_ret_val(ret_val); 
}


// TEST FUNCTIONS ^^

void func_init()
{
    FuncDef *func = NULL;

    g_func_table.first = NULL;
    g_func_table.last = NULL;

    // tuby function
    g_tuby_func.name = "tuby";
    g_tuby_func.native = tuby;
    g_tuby_func.params = 0;
    func_def(&g_tuby_func);

    // print function
    g_tuby_print_int.name = "print";
    g_tuby_print_int.native = print;
    g_tuby_print_int.params = (vector*)malloc(sizeof(vector));
    vector_init(g_tuby_print_int.params);
    vector_push(g_tuby_print_int.params, IntType);
    func_def(&g_tuby_print_int);

    // print(boolean);
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("print");
    func->native = print;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = NULL;
    vector_init(func->params);
    vector_push(func->params, BoolType);
    func_def(func);

    // int + int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("+");
    func->native = add_int;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, IntType);
    vector_push(func->params, IntType);
    func_def(func);

     // int * int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("*");
    func->native = add_int;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = IntType;
    vector_init(func->params);
    vector_push(func->params, IntType);
    vector_push(func->params, IntType);
    func_def(func);


    // int == int;
    func = (FuncDef*)malloc(sizeof(FuncDef));
    func->name = strdup("==");
    func->native = equals_int;
    func->params = (vector*)malloc(sizeof(vector));
    func->value_type = BoolType;
    vector_init(func->params);
    vector_push(func->params, IntType);
    vector_push(func->params, IntType);
    func_def(func);

}

//-----------------------------------------------------------------------------

void func_def(FuncDef *func)
{
    struct FuncTableEntry *entry;

    entry = (struct FuncTableEntry*)malloc(sizeof(struct FuncTableEntry));

    entry->function = func;
    entry->next = 0;
    if (g_func_table.last == 0)
    {
        g_func_table.last = entry;
        g_func_table.first = entry;
    }
    else
    {
        g_func_table.last->next = entry;
        g_func_table.last = entry;
    }
}

//-----------------------------------------------------------------------------

FuncDef *func_get(const char *name, vector *params)
{
    struct FuncTableEntry *e = g_func_table.first;
    while (e != 0)
    {
        if (strcmp(name, e->function->name) == 0)
        {
            if (vector_length(params) == vector_length(e->function->params))
            {   
                char ok = 1;
                int length = vector_length(params);
                int i;
                
                for (i = 0; i < length; i++)
                {
                     ValueType *t1 = vector_at(e->function->params, i);
                     AstNode *expr = vector_at(params, i);
                     ValueType *t2 = expr->value_type;  
                     if (t1 != t2)
                     {
                        ok = 0;
                        break;
                     }     
                }
                
                if (ok == 1)
                    return e->function;
            }
        }

        e = e->next;
    }
    return 0;
}
