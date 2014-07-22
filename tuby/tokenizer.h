/* 
 * Copyright (C) 2014 Toth Arpad 
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* 
 * Enumeration of all possible token types.
 */
enum TokenType
{
    ttNumber,
    ttSemilcon,
    ttIdentifier,
    ttOpenBracket,
    ttCloseBracket,
    ttOpenCurly,
    ttCloseCurly,
    ttOpenSquare,
    ttCloseSquare,
    ttAssign,
    ttAdd,
    ttSub,
    ttMul,
    ttDiv,
    ttMod,
    ttEquals,
    ttNotEquals,
    ttInc,
    ttDec,
    ttLess,
    ttGreater,
    ttTrue,
    ttFalse,
    ttIF,
    ttElse,
    ttWhile,
    ttFor,
    ttDo,
    ttEOF
};

/* 
 * Definition of a source file. Basically a bounch of characters that will be
 * tokenize and parsed into meaningfull structures.
 */
struct SourceFile
{
    const char *buffer; /* Stream of chars. The file content.*/
    int pos;     /* Current position in stream.*/
    int line;    /* Current line*/
    int row;     /* Current row.*/
    int length;  /* Lenght of this file*/
    int current; /* The current character. If -1 it means EOF reached. */
};


/* A lexical unit in the source file. Can be a number an identifier etc..*/
struct Token
{
    enum TokenType type; /* Type of this token. */
    char repr[255]; /* Token representation. */
};

extern struct SourceFile g_source;
extern struct Token g_token;

/* Load source file from file.*/
void load_from_file(const char *file_name);

/* Load source file from string.*/
void load_from_string(const char *string);

int next_token();

/*
 * Take a look at the character that is at the current_position + pos. If 
 * the position is out of stream simply return -1. 
 */
int token_peek(int pos);

int read_number();

int read_open_bracket();

int read_closed_bracket();

int read_semilcon();

int read_identifier();


/*
 * Reat the multiplier operator from the token stream.
 */
int read_mul();

/*
 * Read the add operator from the token stream (+).
 */
int read_add();

int read_less();
int read_greater();
int read_equals();

/*
 * Read assignment token from the stream.
 */
int read_assign();

int read_open_curly();

int read_close_curly();

/* Get a copy of the token representation. */
char *get_token_repr();


#endif // _TOKENZIER_H_
