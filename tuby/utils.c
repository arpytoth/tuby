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

#include "utils.h"
#include "tokenizer.h"

#include <stdlib.h>
#include <stdio.h>

void error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Error at Line %i, Col %i: ", g_source.line, g_source.row);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

void parse_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Parse error at Line %i, Col %i: ", g_source.line, g_source.row);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}
