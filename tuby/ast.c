#include <stdlib.h>
#include "ast.h"
#include "type_map.h"

AstNode *ast_add(AstNode *t1, AstNode *t2)
{
    AstNode *add = (AstNode*)malloc(sizeof(AstNode));

    add->content.bin_terms.term1 = t1;
    add->content.bin_terms.term2 = t2;
    add->type = antAdd;
    
    return add;
}


AstNode *ast_mul(AstNode *t1, AstNode *t2)
{
    AstNode *mul = (AstNode*)malloc(sizeof(AstNode));

    mul->content.bin_terms.term1 = t1;
    mul->content.bin_terms.term2 = t2;
    mul->type = antMul;
    
    return mul;
}


AstNode *ast_bool_val(int value)
{
    AstNode *bool_val_node = (AstNode*)malloc(sizeof(AstNode));

    bool_val_node->content.bool_val = value;
    bool_val_node->type = antBoolVal;
    bool_val_node->value_type = BoolType;
    return bool_val_node;
}


