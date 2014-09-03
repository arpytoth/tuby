#ifndef _TUBY_SOURCEFILE_H_
#define _TUBY_SOURCEFILE_H_

/* 
 * Definition of a source file. Basically a bounch of characters that will be
 * tokenize and parsed into meaningfull structures.
 */
typedef struct SourceFile
{
    const char *buffer; /* Stream of chars. The file content.*/
    int pos;     /* Current position in stream.*/
    int line;    /* Current line*/
    int row;     /* Current row.*/
    int length;  /* Lenght of this file*/
    int current; /* The current character. If -1 it means EOF reached. */
} SourceFile;


extern SourceFile g_source;


#endif // _TUBY_SOURCEFILE_H_
