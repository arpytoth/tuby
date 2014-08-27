#ifndef _TUBY_RED_BLACK_H_
#define _TUBY_RED_BLACK_H_

typedef enum NodeColor
{
    ncBlack, ncRead
} NodeColor;

typedef struct RedBlackNode
{
    struct RedBlackNode *left;
    struct RedBlackNode *right;
    struct RedBlackNode *parent;
    NodeColor color;
    char *key;
    void *value;
} RedBlackNode;

RedBlackNode *redblack_new_node();

typedef struct RedBlack
{
    RedBlackNode *root;
    RedBlackNode *nil;
} RedBlack;

void redblack_insert();


#endif // _TUBY_RED_BLACK_H_
