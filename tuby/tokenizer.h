#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* Enumeration of all possible token types.*/
enum TokenType
{
    ttNumber,
    ttSemilcon,
    ttIdentifier,
    ttOpenBracket,
    ttCloseBracket,
    ttAssign,
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

int read_number();

int read_open_bracket();

int read_closed_bracket();

int read_semilcon();

int read_identifier();

/*
 * Read assignment token from the stream.
 */
int read_assign();

/* Get a copy of the token representation. */
char *get_token_repr();


#endif // _TOKENZIER_H_
