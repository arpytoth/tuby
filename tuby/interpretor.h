#ifndef _INTERPRETOR_H_
#define _INTERPRETOR_H_

#include "ast.h"

/* 
 * Interpret the Tuby application that is currently loaded into the memory. The
 * application is loaded in form of an AST and the root node is g_root.
 */
void interpret();

void interpret_node(AstNode *node);

/*
 * This will evaluate an expression and return the result of the evaluation. 
 */
Value *eval(AstNode *node);

#endif // _INTERPRETOR_H_
