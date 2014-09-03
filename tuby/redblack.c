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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack.h"

////////////////////////////////////////////////////////////////////////////////
//                            INTERNAL STUFF                                  //
////////////////////////////////////////////////////////////////////////////////

typedef enum NodeColor
{
    ncRED, ncBLACK
} NodeColor;

typedef struct RedBlackNode
{
    struct RedBlackNode *parent;
    struct RedBlackNode *left;
    struct RedBlackNode *right;
    NodeColor color;
    void *key;
    void *value;
} RedBlackNode;


////////////////////////////////////////////////////////////////////////////////
//                       CONSTRUCTORS AND DESTRUCTORS                         //
////////////////////////////////////////////////////////////////////////////////

RedBlackTree *rb_new(int (*compare)(void *, void *), 
    void (*freekey)(void *), int (*print)(char *, void*))
{
    RedBlackTree *tree;

    tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    tree->root = NULL;
    tree->compare = compare;
    tree->print = print;
    tree->free = freekey;
    return tree;
}

////////////////////////////////////////////////////////////////////////////////
//                                   UTILS                                    //
////////////////////////////////////////////////////////////////////////////////

RedBlackNode *rb_new_node()
{
    RedBlackNode *node;
    node = (RedBlackNode *)malloc(sizeof(RedBlackNode));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->key = NULL;
    node->value = NULL;
    return node;
}

RedBlackNode *rb_successor(RedBlackNode *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    else if (n->right != NULL)
    {
        RedBlackNode *s = n->right;
        while (s->left != NULL)
            s = s->left;
        return s;
    }
    else
    {
        RedBlackNode *s = n->parent;
        RedBlackNode *ch = n;
        while (s != NULL && ch == s->right)
        {
            ch = s;
            s = s->parent;
        }
        return s;
    }
}

void rb_set_color(RedBlackNode *p, NodeColor color)
{
    if (p != NULL)
        p->color = color;
}

NodeColor rb_color_of(RedBlackNode *p)
{
    if (p == NULL)
        return ncBLACK;
    else
        return p->color;
}

RedBlackNode *rb_parent_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->parent;
}

RedBlackNode *rb_left_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->left;
}

RedBlackNode *rb_right_of(RedBlackNode *p)
{
    return (p == NULL) ? NULL : p->right;
}

int rb_pow2(int pow)
{
    int result = 1;
    int i;
    
    for (i = 1; i <= pow; i++)
        result *= 2;
    
    return result;
}

int rb_depth(RedBlackNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    else
    {
        int depth = 1;
        int left = 0;
        int right = 0;
        
        if (node->left != NULL)
            left = rb_depth(node->left);

        if (node->right != NULL)
            right = rb_depth(node->right);
       
       if (left > right)
            return depth + left;
        else 
            return depth + right;
    }   
}

void rb_right_rotate(RedBlackTree *tree, RedBlackNode *p)
{
    if (p != NULL)
    {
        RedBlackNode *l = p->left;
        p->left= l->right;
        if (l->right != NULL)
            l->right->parent = p;
        l->parent = p->parent;
        if (p->parent == NULL)
            tree->root = l;
        else if (p->parent->right == p)
            p->parent->right = l;
        else
            p->parent->left = l;

        l->right = p;
        p->parent = l;
    }
}

void rb_left_rotate(RedBlackTree *tree, RedBlackNode *p)
{
    if (p != NULL)
    {
        RedBlackNode *r = p->right;
        p->right = r->left;
        if (r->left != NULL)
            r->left->parent = p;
        r->parent = p->parent;
        if (p->parent == NULL)
            tree->root = r;
        else if (p->parent->left == p)
            p->parent->left = r;
        else
            p->parent->right = r;

        r->left = p;
        p->parent = r;
    }
}


////////////////////////////////////////////////////////////////////////////////
//                                   INSERT                                   //
////////////////////////////////////////////////////////////////////////////////

void rb_fix_insert(RedBlackTree *tree, RedBlackNode *x)
{
    /*
     * Algorithm from CLR.
     */

    x->color = ncRED;
    while (x != NULL && x != tree->root && x->parent->color == ncRED)
    {
        if (rb_parent_of(x) == rb_left_of(rb_parent_of(rb_parent_of(x))))
        {
            RedBlackNode *y = rb_right_of(rb_parent_of(rb_parent_of(x)));
            if (rb_color_of(y) == ncRED) 
            {
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(y, ncBLACK);
                rb_set_color(rb_parent_of(rb_parent_of(x)), ncRED);
                x = rb_parent_of(rb_parent_of(x));
            }
            else
            {
                if (x == rb_right_of(rb_parent_of(x)))
                {
                    x = rb_parent_of(x);
                    rb_left_rotate(tree, x);
                }
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(rb_parent_of(rb_parent_of(x)), ncRED);
                rb_right_rotate(tree, rb_parent_of(rb_parent_of(x)));
            }
        }
        else
        {
            RedBlackNode *y = rb_left_of(rb_parent_of(rb_parent_of(x)));
            if (rb_color_of(y) == ncRED) 
            {
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(y, ncBLACK);
                rb_set_color(rb_parent_of(rb_parent_of(x)), ncRED);
                x = rb_parent_of(rb_parent_of(x));
            }
            else
            {
                if (x == rb_left_of(rb_parent_of(x)))
                {
                    x = rb_parent_of(x);
                    rb_right_rotate(tree, x);
                }
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(rb_parent_of(rb_parent_of(x)), ncRED);
                rb_left_rotate(tree, rb_parent_of(rb_parent_of(x)));
            }
        }
    }   
    tree->root->color = ncBLACK;
}

void rb_insert(RedBlackTree *tree, void *key, void *value)
{
    RedBlackNode *t = tree->root;
   
    if (tree->root == NULL)
    { 
        RedBlackNode *node;
        node = rb_new_node();
        node->key = key;
        node->value = value;

        tree->root = node;
        node->color = ncBLACK;
    }
    else
    {
        int cmp;
        RedBlackNode *parent;
        RedBlackNode *e;

        do
        {
            parent = t;
            cmp = tree->compare(key, parent->key);
            if (cmp < 0)
                t = t->left;
            else if (cmp > 0)
                t = t->right;
            else
                return;
        } while (t != NULL);
        
        e = rb_new_node();
        e->key = key;
        e->value = value;
        if (cmp < 0)
            parent->left = e;
        else
            parent->right = e;
        e->parent = parent;
        rb_fix_insert(tree, e);
    }
}

////////////////////////////////////////////////////////////////////////////////
//                                 SEARCH                                     //
////////////////////////////////////////////////////////////////////////////////

RedBlackNode *rb_search_node(RedBlackTree *tree, void *key)
{
    RedBlackNode *p = tree->root;
    while (p != NULL)
    {
        int cmp = tree->compare(key, p->key);
        if (cmp < 0)
            p = p->left;
        else if (cmp > 0)
            p = p->right;
        else
            return p;
    }
    return NULL;
}

void *rb_search(RedBlackTree *tree, void *key)
{
    RedBlackNode *node = rb_search_node(tree, key);
    if (node != NULL)
        return node->value;
    else
        return NULL;
}

////////////////////////////////////////////////////////////////////////////////
///                                DELETE                                     //
////////////////////////////////////////////////////////////////////////////////

void rb_fix_delete(RedBlackTree *tree, RedBlackNode *x)
{   
    /*
     * Algorithm found in CLR.
     */

    while (x != tree->root && x->color == ncBLACK)
    {
        if (x == rb_left_of(rb_parent_of(x)))
        {
            RedBlackNode *s = rb_right_of(rb_parent_of(x));
            if (rb_color_of(s) == ncRED)
            {
                rb_set_color(s, ncBLACK);
                rb_set_color(rb_parent_of(x), ncRED);
                rb_left_rotate(tree, rb_parent_of(x));
                s = rb_right_of(rb_parent_of(x));
            }
            
            if (rb_color_of(rb_left_of(s)) == ncBLACK &&
                rb_color_of(rb_right_of(s)) == ncBLACK) 
            {
                rb_set_color(s, ncRED);
                x = rb_parent_of(x);
            }
            else
            {
                if (rb_color_of(rb_right_of(s)) == ncBLACK)
                {
                    rb_set_color(rb_left_of(s), ncBLACK);
                    rb_set_color(s, ncRED);
                    rb_right_rotate(tree, s);
                    s = rb_right_of(rb_parent_of(x));
                }

                rb_set_color(s, rb_color_of(rb_parent_of(x)));
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(rb_right_of(s), ncBLACK);
                rb_left_rotate(tree, rb_parent_of(x));
                x = tree->root;
            }

        }
        else // symetric
        {
            RedBlackNode *sib = rb_left_of(rb_parent_of(x));
            if (rb_color_of(sib) == ncRED)
            {
                rb_set_color(sib, ncBLACK);
                rb_set_color(rb_parent_of(x), ncRED);
                rb_right_rotate(tree, rb_parent_of(x));
                sib = rb_left_of(rb_parent_of(x));
            }

            if (rb_color_of(rb_right_of(sib)) == ncBLACK &&
                rb_color_of(rb_left_of(sib)) == ncBLACK)
            {
                rb_set_color(sib, ncRED);
                x = rb_parent_of(x);
            }
            else
            {
                if (rb_color_of(rb_left_of(sib)) == ncBLACK) 
                {
                    rb_set_color(rb_right_of(sib), ncBLACK);
                    rb_set_color(sib, ncRED);
                    rb_left_rotate(tree, sib);
                    sib = rb_left_of(rb_parent_of(x));
                }

                rb_set_color(sib, rb_color_of(rb_parent_of(x)));
                rb_set_color(rb_parent_of(x), ncBLACK);
                rb_set_color(rb_left_of(sib), ncBLACK);
                rb_right_rotate(tree, rb_parent_of(x));
                x = tree->root;
            }
        }
    }

    rb_set_color(x, ncBLACK);
}

void rb_delete_node(RedBlackTree *tree, RedBlackNode *p)
{
    RedBlackNode *r;

    // Case1. The node to delete has exactly two children. In this case we do
    //        a simple relabeling with the successor and delete de successor 
    //        that will have at most one child.
    if (p->left != NULL && p->right != NULL)
    {
        RedBlackNode *s = rb_successor(p);
        p->key = s->key;
        p->value = s->value;
        p = s;
    }

    // Start fixup at replacement node, if it exists.
    r = (p->left != NULL ? p->left : p->right);
    if (r != NULL)
    {
        // link the replacement to parent
        r->parent = p->parent;
        if (p->parent == NULL)
            tree->root = r;
        else if (p == p->parent->left)
            p->parent->left = r;
        else
            p->parent->right = r;

        // NULL out links so they are OK to use by rb_fix_delete.
        p->left = p->right = p->parent = NULL;
        
        if (p->color == ncBLACK)
            rb_fix_delete(tree, r);
        free(p);
    }
    else if (p->parent == NULL)
    {
        // return if we are the only node.
        tree->root = NULL;
    }
    else
    {
        // No children? Use self as phantom replacement and unlink.
        if (p->color == ncBLACK)
            rb_fix_delete(tree, p);

        if (p->parent != NULL)
        {
            if (p == p->parent->left)
                p->parent->left = NULL;
            else
                p->parent->right = NULL;
            p->parent = NULL;
        }
        free(p);
    }
}

void *rb_delete(RedBlackTree *tree, void *key)
{
    RedBlackNode *node_to_del;
    node_to_del = rb_search_node(tree, key);
    if (node_to_del != NULL)
    {
        void *value = node_to_del->value;
        tree->free(node_to_del->key);
        rb_delete_node(tree, node_to_del);
        return value;
    }
    else
    {
        return NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////
//                             PRINT                                          //
////////////////////////////////////////////////////////////////////////////////

void rb_print_node(RedBlackTree *tree, RedBlackNode *node)
{
    int size = 2;
    int i;

    if (node == NULL)
    {
        printf(" ");
        for (i = 0; i < size; i++)
            printf("x");
    }
    else
    {
        char buffer[20];
        char color;
        int count;
        
        color = node->color == ncRED?'R':'B';
        count = tree->print(buffer, node->key);
        count++;
        printf(" ");
        for (i = 0; i < size-count; i++)
            printf(" ");
        printf("%s", buffer);
        printf("%c", color);
    }
}

void rb_print(RedBlackTree *tree)
{
    RedBlackNode **queue;
    int queue_pos = 0;
    int depth = rb_depth(tree->root);
    int queue_size = depth * rb_pow2(depth - 1);
    int count = 0;
    int level = 0;
    
    queue = (RedBlackNode**)malloc(sizeof(RedBlackNode*) * queue_size);

    queue[queue_pos++] = tree->root;

    while (queue_pos != 0 && level < depth)
    {
        if (count == 0)
        {
            int k;
            int width =  (rb_pow2(depth - level) / 2  - 1) ;

            for (k = 0; k < width; k++)
                printf("   ");  
        }
        else
        {
            int k;
            int width =  (rb_pow2(depth + 1 - level) / 2  - 1) ;

            for (k = 0; k < width; k++)
                printf("   ");  

        }


        RedBlackNode *node = queue[0];
        int i;
        for (i = 1; i < queue_pos; i++)
            queue[i-1] = queue[i];
        queue_pos--;

        rb_print_node(tree, node);
        
        if (node != NULL)
        {
            queue[queue_pos++] = node->left;
            queue[queue_pos++] = node->right;
        }
        else
        {
            queue[queue_pos++] = NULL;
            queue[queue_pos++] = NULL;

        }
        count++;
        if (count == rb_pow2(level))
        {
            level++;
            count = 0;
            printf("\n");
        }
    }
    printf("\n");
    free(queue);
}

////////////////////////////////////////////////////////////////////////////////
//                            INT RED-BLACK TREE                              //
////////////////////////////////////////////////////////////////////////////////               

int irb_compare_int(void *a, void *b)
{
    int *i = (int *)a;
    int *j = (int *)b;
    if (*i < *j)
        return -1;
    else if (*i == *j)
        return 0;
    else
        return 1;
}

int irb_print_int(char *buffer, void *a)
{
    int *i = (int *)a;
    return sprintf(buffer, "%d", *i);
}

void irb_free_int(void *a)
{
    int  *i = (int *)a;
    free(i);
}

RedBlackTree *irb_new()
{
    RedBlackTree *tree;
    tree = rb_new(&irb_compare_int, &irb_free_int, &irb_print_int);
    return tree;
}
void irb_insert(RedBlackTree *tree, int key, void *value)
{
    int *pkey = (int*)malloc(sizeof(int));
    *pkey = key;
    rb_insert(tree, pkey, value);
}

void *irb_search(RedBlackTree *tree, int key)
{
    void *value = rb_search(tree, &key);
    return value;
}

void *irb_delete(RedBlackTree *tree, int key)
{
    void *value = rb_delete(tree, &key);
    return value;
}

////////////////////////////////////////////////////////////////////////////////
//                           STRING RED-BLACK TREE                            //
////////////////////////////////////////////////////////////////////////////////               

int srb_compare_string(void *a, void *b)
{
    char *i = (char *)a;
    char *j = (char *)b;
    return strcmp(i, j);
}

int srb_print_string(char *buffer, void *a)
{
    char *i = (char *)a;
    return sprintf(buffer, "%s", i);
}

void srb_free_string(void *a)
{
    char  *i = (char *)a;
    free(i);
}

RedBlackTree *srb_new()
{
    RedBlackTree *tree;
    tree = rb_new(&srb_compare_string, &srb_free_string, &srb_print_string);
    return tree;
}

void  srb_insert(RedBlackTree *tree, const char *key, void *value)
{
    char *pkey = strdup(key);
    rb_insert(tree, pkey, value);
}

void *srb_search(RedBlackTree *tree, char *key)
{
    return rb_search(tree, key);
}

void *srb_delete(RedBlackTree *tree, char *key)
{
    return rb_delete(tree, key);
}
