#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "func_table.h"
#include "stack.h"

/* An entry in the function table. */
struct FuncTableEntry
{
    struct FuncDef *function; /* Function implementation. */
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


// TEST FUNCTIONS vv

struct FuncDef g_tuby_func;
void tuby()
{
    printf("Tuby!!!");
}

struct FuncDef g_tuby_print_int;
void print()
{
    Value *param1 = stack_function_param(0);
    int int_val = param1->data.int_val;
    printf("%d\n", int_val);
}

// TEST FUNCTIONS ^^

void init_func_table()
{
    g_func_table.first = 0;
    g_func_table.last = 0;

    // tuby function
    g_tuby_func.name = "tuby";
    g_tuby_func.native = tuby;
    g_tuby_func.params = 0;
    insert_func(&g_tuby_func);

    // print function
    // tuby function
    g_tuby_print_int.name = "print";
    g_tuby_print_int.native = print;
    g_tuby_print_int.params = (vector*)malloc(sizeof(vector));
    vector_init(g_tuby_print_int.params);
    vector_push(g_tuby_print_int.params, &IntType);
    insert_func(&g_tuby_print_int);
}

//-----------------------------------------------------------------------------

void insert_func(struct FuncDef *func)
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

struct FuncDef *get_func(const char *name, vector *params)
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
