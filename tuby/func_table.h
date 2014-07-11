#ifndef _FUNC_TABLE_H_
#define _FUNC_TABLE_H_

#include "parser.h"
#include "vector.h"

/*
 * Initialize the functions table.
 */
void func_init();

/*
 * Insert a new function into the functions table.
 */
void func_def(struct FuncDef *func);

/*
 * Search for the method with the specified name.
 */
struct FuncDef *func_get(const char *name, vector *params);



#endif //_FUNCTION_TABLE_H_
