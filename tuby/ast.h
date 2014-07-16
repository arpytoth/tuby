#ifndef _AST_H_
#define _AST_H_

#include "vector.h"
#include "list.h"

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

struct AstNodeT;
typedef struct AstNodeT AstNode;


/* Enumeration of all primitive types.*/
enum PrimitiveType
{
    ptVoid,
    ptBool,
    ptInt
};


/* Type of a value.*/
typedef struct
{
    /* Name of this value type. For example: int, bool ...*/
    char *name;

    /* Base primitive type of this type.*/
    enum PrimitiveType primitive;
} ValueType;


/*
 * An union of all possible values in the Tuby language.
 */
typedef union
{
    int int_val;
    int bool_val;
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

    ValueType *val_type;
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
    
    /* 0 for false, 1 for true.*/
    int bool_val;
    Return ret;
    If if_;
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

/*
 * Create a new operation node between the two terms, t1 and t2. 
 * Basically the resultant node will be a binary operator of two terms such
 * as t1+t2 or t1==t2.. 
 */
AstNode *ast_binary(const char *symbol, AstNode *t1, AstNode *t2);


/*
 * Create a new BoolValue node for the specified boolean value.
 * value = 0 means false, value = 1 means true.
 */
AstNode *ast_bool_val(int value);

/*
 * Create a new Variable Declaration node that will create a variable with
 * the specified name and type.
 */
AstNode *ast_var_decl(char *name, ValueType *value_type);

AstNode *ast_if(AstNode *cond, AstNode *then, AstNode *els);
#endif // _AST_H_ 
