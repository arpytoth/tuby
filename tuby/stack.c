#include <stdlib.h>
#include "stack.h"
#include "utils.h"

/* The execution stack used by the Tuby program. */
Stack g_stack;

//------------------------------------------------------------------------------

void stack_init()
{
    static int STACK_SIZE = 100;
    g_stack.pos = 0;
    g_stack.size = STACK_SIZE;
    g_stack.data = (StackFrame*)malloc(sizeof(StackFrame) * STACK_SIZE);
}

//------------------------------------------------------------------------------

StackFrame *stack_push()
{
    if (g_stack.pos < g_stack.size - 1)
    {
        g_stack.pos++;
        return &g_stack.data[g_stack.pos];
    }
    return NULL;
}

//------------------------------------------------------------------------------

StackFrame *stack_pop()
{
    if (g_stack.pos >= 0)
    {
        g_stack.pos--;
        return &g_stack.data[g_stack.pos + 1];
    }
    return NULL;
}

//------------------------------------------------------------------------------

StackFrame *stack_top()
{
    if (g_stack.pos >= 0)
    {
        return &g_stack.data[g_stack.pos];
    }
    return NULL;
}

//------------------------------------------------------------------------------

Value *stack_function_param(int index)
{
    StackFrame *top = stack_top();
    if (top != NULL)
    {
        return (Value*)vector_at(&top->params, index);
    }
    else
    {
        return NULL;
    }
}

//------------------------------------------------------------------------------


