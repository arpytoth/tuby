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
#ifndef _TUBY_ARRAY_H_
#define _TUBY_ARRAY_H_

#include "ast.h"
#include "vector.h"


struct Array
{
    vector *data;
};

Value *array_get_val(Value *a, int index);
Value *array_set_val(Value *a, int index, Value *val_to_set);
void array_free(struct Array *arr);
void array_use(struct Array *arr);
void array_copy(struct Array *dest, struct Array *src);

#endif // _TUBY_ARRAY_H_
