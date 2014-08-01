/* 
 * Copyright (C) 2014 Toth Arpad 
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
            g_source.line, g_source.row);
     
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}


/* Check if the provided character is a letter or not. */
int is_letter(const char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    else
        return 0;
}

/* Chec if the specified character is a digit or not. */
int is_digit(const char c)
{
    if (c >= '0' && c <= '9') 
        return 1;
    else
        return 0;
}

/* Check if the specified character is a whitespace or not.*/
int is_whitespace(const char c)
{
    return c == ' ' || c == '\n' ||  c == '\t' || c == '\r';
}

/* Check if EOF reached.*/
int eof()
{
    return g_source.current == -1;
}


int token_peek(int pos)
{
    int actual_pos = g_source.pos + pos;

    if (g_source.pos < g_source.length - 1)
    {
        return g_source.buffer[actual_pos];
    }
    else
    {
        return -1;
    }
}


/* Read the next character in the stream. Return 1 if ok, 0 if EOF reached.*/
int next_char()
{
    if (g_source.pos < g_source.length - 1)
    {
        g_source.pos++;
        g_source.current = g_source.buffer[g_source.pos];
        if (g_source.current == '\n')
        {
            g_source.line++;
            g_source.row = 0;
        }
        else
        {
            g_source.row++;
        }
        return 1;
    }
    else
    {
        g_source.current = -1;
        return 0;
    }
}

/* 
 * Read the next nonwhite character from the stream. Return 1 in case of 
 * success, 0 in case EOF reached.
 */
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
    g_token.repr[1] = '\0'
    next_char();
    return 1;
}

/*
 * Read the next token from file. Return 1 in case of success, 0 in case EOF
 * reached.
 */
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

/* Read the next number token from the stream. */
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
    strncpy(g_token.repr, g_source.buffer + start, size - 1);
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
    strncpy(g_token.repr, g_source.buffer + start, size - 1);
    g_token.repr[size - 1] = '\0';
    g_token.type = ttString;
    next_char();
    return 1;

}



/* Read the next identifier token from the source file.*/
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
    strncpy(g_token.repr, g_source.buffer + start, size - 1);
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


/* Read the ; character from the stream.*/
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


void load_from_string(const char *string)
{
    int length = strlen(string);
    char *buffer = (char*)malloc((length + 1) * sizeof(char));
    strncpy(buffer, string, length);
    buffer[length] = '\0';

    g_source.pos = -1;
    g_source.line = 0;
    g_source.row = 0;
    g_source.buffer = buffer;
    g_source.length = length;
    next_nonwhite();
}

void load_from_file(const char *file_name)
{
    char *source = NULL;
    FILE *fp = fopen(file_name, "r");

    if (fp != NULL) 
    {
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            long bufsize = ftell(fp);
            if (bufsize == -1) 
                return;

            /* Allocate our buffer to that size. */
            source = (char *)malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0)
                return;

            /* Read the entire file into memory. */
            g_source.length = fread(source, sizeof(char), bufsize, fp);
            if (g_source.length == 0) {
                fputs("Error reading file", stderr);
            } else {
                source[g_source.length] = '\0'; /* Just to be safe. */
            }
        }
        fclose(fp);
    }
    g_source.pos = -1;
    g_source.line = 0;
    g_source.row = 0;
    g_source.buffer = source;
    next_nonwhite();
}


char *get_token_repr()
{
    int size = strlen(g_token.repr);
    char *copy = (char*)malloc(size + 1);
    strcpy(copy, g_token.repr);
    copy[size] = '\0';
    return copy;
}
