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

#include "vector.h"
#include "list.h"

/*
 * This file contains the structures used internally by the Tuby 
 * virtual machine along with primitive types and so on.
 */

struct Value;
struct Var;
struct ValueType;

//----------------------------------------------------------------------------//

typedef struct Member 
{
    char *name;
    struct ValueType *val_type;
} Member;

Member *member_create(char *name, struct ValueType *type);
void member_destroy(Member *member);

//----------------------------------------------------------------------------//

typedef struct ValueType
{
    char *name;
    struct ValueType *uval_type; // underlying value type.
    int is_array; // this is an array of uval_type;
    int is_ref; // this is a pointer to the uval_type;
    int is_class; // this is a class type.
    List members;
} ValueType;

ValueType *type_create(char *name);
void type_destroy(ValueType *type);
void type_add_member(ValueType *type, Member *member);
Member *type_get_member(ValueType *type, const char *member);

//----------------------------------------------------------------------------//

typedef struct Var
{
    char *name;
    ValueType *val_type;
    struct Value *val;
} Var;

Var *var_create(char *name, ValueType *val_type);
void var_init(Var *var, char *name, ValueType *val_type);
void var_set(Var *var, struct Value *val);
void var_free(Var *var);

//----------------------------------------------------------------------------//

typedef struct Array
{
    vector *data;
} Array;

//----------------------------------------------------------------------------//

typedef struct String
{
    int len;
    char *buffer;
} String;
String *str_create();

//----------------------------------------------------------------------------//

typedef struct Object
{
    List *members;
} Object;

void obj_init(Object *obj, ValueType *val_type);
void obj_destroy(Object *obj);
void obj_free(Object *obj);
void obj_use(Object *obj);
void obj_copy(Object *dest, Object *src);
Var *obj_get_var(Object *obj, char *name);

//----------------------------------------------------------------------------//

union AllValues
{
    int int_val;
    int bool_val;
    char char_val;
    struct Array array_val;
    struct String str_val;
    Object obj_val;
};

typedef struct Value
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
    
    Var *var; /* If set to a non NULL value it means this value is linked to
                 a variable and var must be set with the value. */
    int index;
} Value;

#endif // _TUBY_H_

