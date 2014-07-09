#ifndef _STACK_H_
#define _STACK_H_

#include "vector.h"
#include "ast.h"

/* A frame in The Stack. Holds the informations about a function call. */
typedef struct StackFrameT
{
    /* List of called parameters. */
    vector params;
} StackFrame;

/* The execution stack. */
typedef struct StackT
{
    /* Size of the stack. */
    int size;

    /* Current position of the top of the stack. */
    int pos;
    
    /* Preallocated data in this stack. */
    StackFrame *data;
} Stack;

/*
 * Initialize the execution stack.
 */
void stack_init();

/*
 * Pop last frame from the top.
 */
StackFrame *stack_pop();

/*
 * Push a new frame on the stack and return it.
 */
StackFrame *stack_push();

/*
 * Get the top element of the stack without actually removing it. To remove it
 * is stack_pop() function. If NULL is returned, it means the stack is empty.
 */
StackFrame *stack_top();

Value *stack_function_param(int index);
#endif // _STACK_H_

