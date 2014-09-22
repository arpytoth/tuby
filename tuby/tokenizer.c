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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils.h"
#include "tokenizer.h"

/* Global source file used to basically to store the entire source code. */
struct SourceFile g_source;

/*
 * Globally used token. We read one token at time and we have a single process
 * tokenizer. So we can store all tokens in one token.
 */
struct Token g_token;


void token_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Syntax Error:  at Line %i, Col %i: ", 
            g_source.line, g_source.col);
     
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

int is_letter(const char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    else
        return 0;
}

int is_digit(const char c)
{
    if (c >= '0' && c <= '9') 
        return 1;
    else
        return 0;
}

int is_whitespace(const char c)
{
    return c == ' ' || c == '\n' ||  c == '\t' || c == '\r';
}

int eof()
{
    return g_source.current == -1;
}


int token_peek(int pos)
{
    int actual_pos = g_source.pos + pos;

    if (g_source.pos < g_source.size - 1)
    {
        return g_source.data[actual_pos];
    }
    else
    {
        return -1;
    }
}

int next_char()
{
    if (g_source.pos < g_source.size - 1)
    {
        g_source.pos++;
        g_source.current = g_source.data[g_source.pos];
        if (g_source.current == '\n')
        {
            g_source.line++;
            g_source.col= 1;
        }
        else
        {
            g_source.col++;
        }
        return 1;
    }
    else
    {
        g_source.current = -1;
        return 0;
    }
}

int next_nonwhite()
{
    if (!next_char())
        return 0;

    while (is_whitespace(g_source.current))
    {
       if (!next_char())
           return 0;
    }
    return 1;
}


int token_read_char()
{
    char chr;
    
    next_char();
    chr = g_source.current;
    next_char();
    
    if (g_source.current != '\'')
        token_error("Expected ' to close the char constant.");
   
    next_char();
    g_token.repr[0] = chr;
    g_token.repr[1] = '\0';
    g_token.type = ttChar;
    return 1;
}

int token_read_comma()
{
    g_token.type = ttComma;
    g_token.repr[0] = ',';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}

int read_or()
{
    g_token.type = ttOr;
    g_token.repr[0] = '|';
    g_token.repr[1] = '|';
    g_token.repr[2] = '\0';
    next_char();
    next_char();
    return 1;
}

int read_and()
{
    g_token.type = ttAdd;
    g_token.repr[0] = '&';
    g_token.repr[1] = '&';
    g_token.repr[2] = '\0';
    next_char();
    next_char();
    return 1;
}

int next_token()
{
    if (g_source.current < 0)
    {
        g_token.type =  ttEOF;
        return 0;
    }
    
    if (is_whitespace(g_source.current))
        next_nonwhite();

    if (is_digit(g_source.current))
    {
        return read_number();
    }
    else if (g_source.current == ',')
    {
        return token_read_comma();
    }
    else if (g_source.current == '\'')
    {
        return token_read_char();
    }
    else if (g_source.current == '"')
    {
        return token_read_string();
    }
    else if (g_source.current == '(')
    {
        return read_open_bracket();
    }
    else if (g_source.current == ')')
    {
        return read_closed_bracket();
    }
    else if (g_source.current == ';')
    {
        return read_semilcon();
    }
    else if (g_source.current == '{')
    {
        return read_open_curly();
    }
    else if (g_source.current == '}')
    {
        return read_close_curly();
    }
    else if (g_source.current == '[')
    {
        g_token.type = ttOpenSquare;
        next_char();
        return 1;
    }
    else if (g_source.current == ']')
    {
        g_token.type = ttCloseSquare;
        next_char();
        return 1;
    }
    else if (g_source.current == '|' && token_peek(1) == '|')
    {
        return read_or();
    }
    else if (g_source.current == '&' && token_peek(1) == '&')
    {
        return read_and();
    }
    else if (g_source.current == '=' && token_peek(1) == '=')
    {
        return read_equals();        
    }
    else if (g_source.current == '!' && token_peek(1) == '=')
    {
        g_token.type = ttNotEquals;
        next_char();
        next_char();
        return 1;     
    }
    else if (g_source.current == '+' && token_peek(1) == '+')
    {
        g_token.type = ttInc;
        next_char();
        next_char();
        return 1;     
    }
    else if (g_source.current == '-' && token_peek(1) == '-')
    {
        g_token.type = ttDec;
        next_char();
        next_char();
        return 1;     
    }
    else if (g_source.current == '=')
    {
        return read_assign();
    }
    else if (g_source.current == '-')
    {
        g_token.type = ttSub;
        next_char();
        return 1;
    }
    else if (g_source.current == '+')
    {
        return read_add();
    }
    else if (g_source.current == '*')
    {
        return read_mul();
    }
    else if (g_source.current == '/')
    {
        g_token.type = ttDiv;
        next_char();
        return 1;
    }
    else if (g_source.current == '%')
    {
        g_token.type = ttMod;
        next_char();
        return 1;
    }
    else if (is_letter(g_source.current))
    {
        return read_identifier();
    }
    else
    {
        g_token.type = ttEOF; 
        return 0;
   }
}

int read_number()
{
    int size = 0;
    int start = g_source.pos;
    int end = start;

    while (is_digit(g_source.current))
    {
        if (next_char() < 0) 
        {
            end = g_source.pos;
            return 0;
        }
    }

    end = g_source.pos - 1;
    size = end - start+ 2;
    strncpy(g_token.repr, g_source.data + start, size - 1);
    g_token.repr[size - 1] = '\0';
    g_token.type = ttNumber;
    return 1;
}

int token_read_string()
{
    int start = g_source.pos + 1;
    int end = start;
    int size = 0;
    next_char();
    while (g_source.current != '"')
    {
        if (next_char() < 0) 
        {
            error("EOF reached!");
            return 0;
        }
    }

    end = g_source.pos - 1;
    size = end - start + 2;
    strncpy(g_token.repr, g_source.data + start, size - 1);
    g_token.repr[size - 1] = '\0';
    g_token.type = ttString;
    next_char();
    return 1;

}

int read_identifier()
{
    int start = g_source.pos;
    int end = start;
    int size = 0;

    while (is_letter(g_source.current))
    {
        if (next_char() < 0) 
        {
            error("EOF reached!");
            return 0;
        }
    }

    end = g_source.pos - 1;

    size = end - start + 2;
    strncpy(g_token.repr, g_source.data + start, size - 1);
    g_token.repr[size - 1] = '\0';
   
    if (strcmp(g_token.repr, "for") == 0)
    {
        g_token.type = ttFor;
    }
    else if (strcmp(g_token.repr, "while") == 0)
    {
        g_token.type = ttWhile;
    }
    else if (strcmp(g_token.repr, "do") == 0)
    {
        g_token.type = ttDo;
    }
    else if (strcmp(g_token.repr, "if") == 0)
    {
        g_token.type = ttIF;
    }
    else if (strcmp(g_token.repr, "else") == 0)
    {
        g_token.type = ttElse;
    }
    else if (strcmp(g_token.repr, "true") == 0)
    {
        g_token.type = ttTrue;
    }
    else if (strcmp(g_token.repr, "false") == 0)
    {
        g_token.type = ttFalse;
    }
    else if (strcmp(g_token.repr, "include") == 0)
    {
        g_token.type = ttInclude;
    }
    else
    {
        g_token.type = ttIdentifier;
    }

    return 1;
}


int read_mul()
{
    g_token.type = ttMul;
    g_token.repr[0] = '*';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}


int read_add()
{
    g_token.type = ttAdd;
    g_token.repr[0] = '+';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}


int read_open_bracket()
{
    g_token.type = ttOpenBracket;
    g_token.repr[0] = '(';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}

int read_closed_bracket()
{
    g_token.type = ttCloseBracket;
    g_token.repr[0] = ')';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}

int read_semilcon()
{
    g_token.type = ttSemilcon;
    g_token.repr[0] = ';';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}

int read_assign()
{
    g_token.type = ttAssign;
    g_token.repr[0] = '=';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}


int read_open_curly()
{
    g_token.type = ttOpenCurly;
    g_token.repr[0] = '{';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}


int read_close_curly()
{
    g_token.type = ttCloseCurly;
    g_token.repr[0] = '}';
    g_token.repr[1] = '\0';
    next_char();
    return 1;
}


int read_equals()
{
    g_token.type = ttEquals;
    g_token.repr[0] = '=';
    g_token.repr[1] = '=';
    g_token.repr[2] = '\0';
    next_char();
    next_char();
    return 1;
}

char *get_token_repr()
{
    int size = strlen(g_token.repr);
    char *copy = (char*)malloc(size + 1);
    strcpy(copy, g_token.repr);
    copy[size] = '\0';
    return copy;
}
