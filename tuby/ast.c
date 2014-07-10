#include <stdlib.h>
#include "ast.h"
ValueType IntType;

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
