#ifndef _TUBY_SOURCEFILE_H_
#define _TUBY_SOURCEFILE_H_

/* 
 * Definition of a source file. Basically a bounch of characters that will be
 * tokenize and parsed into meaningfull structures.
 */
typedef struct SourceFile
{
    char *data; 
    int pos;     
    int line;  
    int col;
    int size;
    int current;
} SourceFile;




#endif // _TUBY_SOURCEFILE_H_
