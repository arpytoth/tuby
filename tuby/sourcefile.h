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


void src_init(SourceFile *file, const char *file_name);
SourceFile *src_load(const char *file_name);
void src_preproc(SourceFile *file);
int src_next(SourceFile *file);

#endif // _TUBY_SOURCEFILE_H_
