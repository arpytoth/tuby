#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "sourcefile.h"

SourceFile g_source;

void preproc_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Preprocessor Error: ");
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

/*
 * This will load the content of a file, line by line and if a line starts with
 * the # character, that means that line contains a precompile directive and
 * we must threat the precompile directive right here in place. 
 *
 * Supported Directives:
 * #include - load the filename after include and append it to the current
 *            stream.
 */
char *load_file(const char *file_name)
{
    char *content = NULL;
    int length;
    
    FILE *fp = fopen(file_name, "r");
    if (fp != NULL) 
    {
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            long bufsize = ftell(fp);
            if (bufsize == -1) 
                return;

            content = (char *)malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0)
                return;

            length = fread(content, sizeof(char), bufsize, fp);
            if (length == 0) 
            {
                preproc_error("Error reading file %s", file_name);
            } 
            else 
            {
                content[length] = '\0'; /* Just to be safe. */
            }
        }
        fclose(fp);
    }
    else
    {
        preproc_error("Could not open file: %s", file_name);
    }
    return content;
}

char preproc_char_at(char *content, int index, int length)
{
    if (index < length && index >= 0)
        return content[index];
    else
        return '\0';
}

char *preproc_file(char *content)
{
    int length = strlen(content);
    int i;
    for (i = 0; i < length; i++)
    {
        char c = preproc_char_at(content, i, length);
        if (c == '#')
        {
            if (preproc_char_at(content, i+1, length) == 'i' &&
                preproc_char_at(content, i+2, length) == 'n' &&
                preproc_char_at(content, i+3, length) == 'c' &&
                preproc_char_at(content, i+4, length) == 'l' &&
                preproc_char_at(content, i+5, length) == 'u' &&
                preproc_char_at(content, i+6, length) == 'd' &&
                preproc_char_at(content, i+7, length) == 'e') 
            {
                printf("Include found");
            }
        }
    }  
    return NULL;
}

int main()
{
    char *content = load_file("program.txt");
    preproc_file(content); 
    return 0;
}

