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

#include "tokenizer.h"
#include "func_table.h"
#include "interpretor.h"
#include "stack.h"
#include "vector.h"
#include "type_map.h"
#include "var_map.h"
#include "string.h"
#include "tuby_array.h"
#include "allocator.h"
#include "tuby_char.h"
#include "tuby_string.h"
#include "tuby_int.h"

int main()
{
    varmap_init();
    stack_init();
    type_map_init();
    func_init();
    
    int_init_module();
    char_init_module();
    string_init_module();

    load_from_file("program.txt");
    parse();
    varmap_release();
    interpret();

    printf("Number of alloc: %d\n", g_alloc_count);
    return 1;
}
