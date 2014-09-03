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
#ifndef _REDBLACK_H_
#define _REDBLACK_H_

////////////////////////////////////////////////////////////////////////////////
//                           GENERIC RED-BLACK TREE                           //
////////////////////////////////////////////////////////////////////////////////


typedef struct RedBlackTree
{
    struct RedBlackNode *root;
    int (*compare)(void *, void *);
    void (*free)(void *);
    int (*print)(char *, void *);
} RedBlackTree;


RedBlackTree *rb_new(int (*compare)(void *, void *), void (*free)(void *),
    int (*print)(char *, void *));
void rb_insert(RedBlackTree *tree, void *key, void *value);
void *rb_search(RedBlackTree *tree, void *key);
void *rb_delete(RedBlackTree *tree, void *key);


////////////////////////////////////////////////////////////////////////////////
//                            INT RED-BLACK TREE                              //
////////////////////////////////////////////////////////////////////////////////               

RedBlackTree *irb_new();
void irb_insert(RedBlackTree *tree, int key, void *value);
void *irb_search(RedBlackTree *tree, int key);
void *irb_delete(RedBlackTree *tree, int key);

////////////////////////////////////////////////////////////////////////////////
//                          STRING KEY RED-BLACK TREE                         //
////////////////////////////////////////////////////////////////////////////////

RedBlackTree *srb_new();
void  srb_insert(RedBlackTree *tree, const char *key, void *value);
void *srb_search(RedBlackTree *tree, char *key);
void *srb_delete(RedBlackTree *tree, char *key);

#endif // _REDBLACK_H_

