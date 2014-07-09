#ifndef _AST_H_
#define _AST_H_

#include "vector.h"

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
struct AstNodeT;
typedef struct AstNodeT AstNode;

/* Enumeration of all primitive types.*/
enum PrimitiveType
{
    ptVoid,
    ptInt
};

/* Type of a value.*/
typedef struct
{
    /* Base primitive type of this type.*/
    enum PrimitiveType primitive;
} ValueType;


/*
 * An union of all possible values in the Tuby language.
 */
typedef union
{
    int int_val;
} AllValues;

/*
 * A value in the Tuby program, basically a meaningfull chunk of memory such as
 * an integer value or a string. Expressions are evaluated into values and 
 * stored into the memory.
 */
typedef struct
{
    /* Type of this value. */
    ValueType *type;

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
    /* Type of this variable. */
    ValueType value_type;

    /* Value assigned to this variable. */
    Value *val;
} Var;


/* 
 * Definition of a function.
 */
struct FuncDef
{
    /* List of parameter types. */    
    vector *params; 

    /* The name of the function. */
    char *name; 
    
    /*
     * Callback to the native implementation of the function. If NULL it 
     * means we don't have a native imlpementation of this function.
     */
    void (*native)(); /* Native function definition. */
};


extern ValueType IntType;

////////////////////////////////////////////////////////////////////////////////
// AST
////////////////////////////////////////////////////////////////////////////////
/* Enumeration of all possible node types.*/
enum AstNodeType
{
    antFuncCall,
    antIntVal,
    antVarDecl,
    antAssign,
    antVarVal,
    antStmtList
};

/* Causes the interpretor to call a function. */
typedef struct FuncCallT
{
    vector params; /* A vector of parameters. */
    struct FuncDef* func; /* The function to be called. */
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
} VarDecl;

typedef struct
{
    char *name;
}VarVal;

/*
 * Instruction that will cause the variable with the specified name to
 * be assigned with the specified expression.
 */
typedef struct
{
    /* Name of the variable.*/
    char *name;

    /* Expression that will be evaluated and then assigned.*/
    AstNode *expr;
} Assign;

/* An entry in an AstNode list. */
struct AstNodeListEntry
{
    struct AstNodeT *stmt;
    struct AstNodeListEntry *next;
};

/* A list of AST nodes.*/
struct AstNodeList
{
    struct AstNodeListEntry *first;
    struct AstNodeListEntry *last;
};

/* Union of all node contents.*/
union AllNodeContent
{
    FuncCall func_call;
    struct AstNodeList stmt_list;
    IntVal int_val;
    Assign assign;
    VarDecl var_decl;
    VarVal var_val;
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

#endif // _AST_H_ 
