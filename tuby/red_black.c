#include <stdlib.h>

typedef enum NodeColor
{
    ncBlack, ncRed
} NodeColor;

typedef struct RedBlackNode
{
    struct RedBlackNode *left;
    struct RedBlackNode *right;
    struct RedBlackNode *parent;
    NodeColor color;
    int key;
} RedBlackNode;

RedBlackNode *redblack_new_node()
{
    RedBlackNode *node;
    node = (RedBlackNode *)malloc(sizeof(RedBlackNode));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = ncBlack;
    node->key = 0;
}

//------------------------------------------------------------------------------

typedef struct RedBlack
{
    RedBlackNode *root;
    RedBlackNode *nil;
} RedBlack;

void redblack_new()
{
    RedBlack *tree;
    tree = (RedBlack *)malloc(sizeof(RedBlack));
    tree->nil = redblack_new_node();
    tree->root = tree->nil;
    return tree;
}

void redblack_insert(RedBlack *tree, RedBlackNode *node)
{
    node->left = tree->nil;
    node->right = tree->nill;
    node->color = ncRed;

    if (tree->root == tree->nil)
    {
        tree->root = node;
    }
    else
    {
        RedBlackNode *parent = root;
        
        while (parent != NULL)
        {
            if (node->key < parent->key)
            {
                if (parent->left = nil)
                {
                    parent->left = node;
                    break;
                }
                else
                {
                    parent = parent->left;
                }
            }
            else if (node->key == parent->key)
            {
                break;
            }
            else 
            {    
                if (parent->right = nil)
                {
                    parent->right = node;
                    break;
                }
                else
                {
                    parent = parent->right;
                }
            }
        }
    }
}



int main()
{
    return 0;
}
