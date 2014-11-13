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
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "source.h"

enum TokenType
{
    ttString, ttNumber, ttChar, ttSemilcon, ttComma, ttIdentifier, 
    ttOpenBracket, ttCloseBracket, ttOpenCurly, ttCloseCurly, 
    ttOpenSquare, ttCloseSquare, ttAssign, ttAdd, ttSub, ttMul, 
    ttDiv, ttMod, ttEquals, ttNotEquals, ttAnd, ttOr, ttInc, ttDec, ttLess, 
    ttGreater, ttTrue, ttFalse, ttIF, ttElse, ttWhile, ttFor,
    ttDo, ttEOF, ttInclude, ttReturn, ttClass, ttDot
};


/* A lexical unit in the source file. Can be a number an identifier etc..*/
struct Token
{
    enum TokenType type; /* Type of this token. */
    char repr[255]; /* Token representation. */
};

extern struct Token g_token;

int next_token();

/*
 * Take a look at the character that is at the current_position + pos. If 
 * the position is out of stream simply return -1. 
 */
int token_peek(int pos);

int read_number();
int token_read_string();
int read_open_bracket();
int read_closed_bracket();
int read_semilcon();
int read_identifier();
int read_mul();
int read_add();
int read_less();
int read_greater();
int read_equals();
int read_assign();
int read_open_curly();
int read_close_curly();
char *get_token_repr();


#endif // _TOKENZIER_H_
