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

#ifndef _TUBY_H_
#define _TUBY_H_

/*
 * This file contains the structures used internally by the Tuby 
 * virtual machine along with primitive types and so on.
 */




typedef struct ValueType
{
    char *name;
    struct ValueType *uval_type; // underlying value type.
    int is_array; // this is an array of uval_type;
    int is_ref; // this is a pointer to the uval_type;
} ValueType;

struct Value;
typedef struct Value Value;

union AllValues
{
    int int_val;
    int bool_val;
    char char_val;
    struct Array array_val;
    struct String str_val;
    Object obj_val;
};


struct Value
{
    ValueType *value_type;
    union AllValues data;
    int ref_count;
    int is_null;
    
    /*
     * The value this value is part of. For example if this value is
     * part of an array or a string, the parent_value will be the
     * array or string value.
     */
    struct Value *parent;
    int index;
};


typedef struct Var
{
    char *name;
    ValueType *val_type;
    Value *val;
} Var;


#endif // _TUBY_H_

