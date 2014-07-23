/* 
 * Copyright (C) 2014 Toth Arpad 
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *    
 *   Toth Arpad (arpytoth@yahoo.com)
 */

#ifndef _AST_H_
#define _AST_H_

#include "vector.h"
#include "list.h"

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

struct AstNodeT;
typedef struct AstNodeT AstNode;

/*
 * There can be derived types from a value type. Primitive means the actual
 * value type but for example vtArray means an array of that type.
 */
typedef enum
{
    vtPrimitive,
    vtArray
} ValueTypeType;

/*
 * Strong type in Tuby. Such as int, bool or anything else.
 */
typedef struct
{
    /* Name of this value type. For example: int, bool ...*/
    char *name;
    ValueTypeType type;    
} ValueType;


/*
 * An union of all possible values in the Tuby language.
 */
typedef union
{
    int int_val;
    int bool_val;
    vector vector_val;
} AllValues;


/*
 * A value in the Tuby program, basically a meaningfull chunk of memory such as
 * an integer value or a string. Expressions are evaluated into values and 
 * stored into the memory.
 */
typedef struct
{
    /* Type of this value. */
    ValueType *value_type;

    /* The data of this value. */
    AllValues data;

    /* Number of actual variables  that are currently using this value. */
    int ref_count;
} Value;


/* 
 * A variable from the Tuby program. 
 */
typedef struct
{
    char *name;

    /* Type of this variable. */
    ValueType *val_type;

    /* Value assigned to this variable. */
    Value *val;
} Var;


/* 
 * Definition of a function.
 */
typedef struct
{
    /* List of parameter types. */    
    vector *params; 

    /* The name of the function. */
    char *name; 
   
    /* The return value of this function. */
    ValueType *value_type;

    /*
     * Callback to the native implementation of the function. If NULL it 
     * means we don't have a native imlpementation of this function.
     */
    void (*native)(); /* Native function definition. */
} FuncDef;


////////////////////////////////////////////////////////////////////////////////
// AST
////////////////////////////////////////////////////////////////////////////////
/* Enumeration of all possible node types.*/
enum AstNodeType
{
    antFuncCall,
    antIntVal,
    antBoolVal,
    antVarDecl,
    antAssign,
    antVarVal,
    antReturn,
    antIf,
    antFor,
    antWhile,
    antStmtList
};


/* Causes the interpretor to call a function. */
typedef struct FuncCallT
{
    vector params; /* A vector of parameters. */
    FuncDef* func; /* The function to be called. */
} FuncCall;


/* An integer literal value in a Tuby Program.*/
typedef struct
{
    /* The actual int value. */
    int value;
} IntVal;


/*
 * Instruction that will cause the creation of a new variable with the
 * specified name;
 */
typedef struct
{
    /* Name of the variable. */
    char *name;

    /**/
    ValueType *val_type;

    /* Initializer. */
    AstNode *init;
} VarDecl;


/* Evaluated into the value of the specified variable. */
typedef struct
{
    /* Name of the variable to get value from. */
    char *name;

    /* Index in case of array or map. */
    AstNode *index;

} VarVal;


/*
 * Instruction that will cause the variable with the specified name to
 * be assigned with the specified expression.
 */
typedef struct
{
    /* Name of the variable.*/
    char *name;
    
    /* Index where to assign value, in case of array.*/
    AstNode *index;

    /* Expression that will be evaluated and then assigned.*/
    AstNode *expr;
    
} Assign;


/*
 * The IF statement. Basically this must have a condition, an then 
 * branch and an else brach.
 */
typedef struct
{
    /* 
     * Boolean condition that if is true the then statement is
     * executed
     */
    AstNode *cond;

    /*  Executed if cond is true */
    AstNode *then;

    /* Executed if cond is false.*/
    AstNode *els;
} If;


/*
 * The While instruction. Basically the classical loop instruction
 * executed by the interpretor.
 */
typedef struct
{
    /* The while condition. */
    AstNode *cond;

    /* What to execute while the while condition is true */
    AstNode *body;
} While;


typedef struct
{
    AstNode *init;
    AstNode *cond;
    AstNode *inc;
    AstNode *body;
} For;

/*
 * Instruction that causes a subprogram to exit and also sets the return
 * value in the stack frame.
 */
typedef struct
{
    /* Will be evaluated into the return value. */
    AstNode *ret_val;
} Return;

/* Union of all node contents.*/
union AllNodeContent
{
    FuncCall func_call;
    list stmt_list;
    IntVal int_val;
    Assign assign;
    VarDecl var_decl;
    VarVal var_val;
    int bool_val;
    Return ret;
    If if_;
    While while_;
    For for_;
};


/* 
 * Well this is basically a node in the abstract syntax tree. The job of the
 * parser is to convert from tokens to AstNodes. Then the AST nodes are fed to
 * the interpretor
 */
struct AstNodeT
{
    enum AstNodeType type; /* The type of this node. */
    union AllNodeContent content; /* Data associated with this node. */
    
    /* Data type of this expression. If expression. */
    ValueType *value_type;
};

////////////////////////////////////////////////////////////////////////////////
// AST functions.
////////////////////////////////////////////////////////////////////////////////

AstNode *ast_binary(const char *symbol, AstNode *t1, AstNode *t2);
AstNode *ast_unary(const char *symbol, AstNode *t);
AstNode *ast_bool_val(int value);
AstNode *ast_var_decl(char *name, ValueType *value_type, AstNode *init);
AstNode *ast_if(AstNode *cond, AstNode *then, AstNode *els);
AstNode *ast_while(AstNode *cond, AstNode *body);
AstNode *ast_for(AstNode *init, AstNode *cond, AstNode *inc, AstNode *body);
AstNode *ast_varval(Var *var);
AstNode *ast_assign(Var *ref, AstNode *val);

#endif // _AST_H_ 
