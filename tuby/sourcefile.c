#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "list.h"
#include "sourcefile.h"

void src_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Preprocessor Error: ");
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

void src_reset(SourceFile *file)
{
    file->current = -1;
    file->pos = -1;
    file->line = 0;
    file->col = 0;
}

void src_free(SourceFile *file)
{
    free(file->data);
    free(file);
}

void src_init(SourceFile *file, const char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp != NULL) 
    {
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            long bufsize = ftell(fp);
            if (bufsize == -1) 
                src_error("Error reading file %s", file_name);

            file->data = (char*)malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0)
                src_error("Error reading file");

            file->size = fread(file->data, sizeof(char), bufsize, fp);
            if (file->size == 0) 
                src_error("Error reading file %s", file_name);
            else 
                file->data[file->size] = '\0'; 

            src_reset(file);
        }
        fclose(fp);
    }
    else
    {
        src_error("Could not open file: %s", file_name);
    }
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
SourceFile *src_load(const char *file_name)
{
    SourceFile *file = NULL;
    
    FILE *fp = fopen(file_name, "r");
    if (fp != NULL) 
    {
        if (fseek(fp, 0L, SEEK_END) == 0) 
        {
            long bufsize = ftell(fp);
            if (bufsize == -1) 
                src_error("Error reading file %s", file_name);

            file = (SourceFile *)malloc(sizeof(SourceFile));
            file->data = (char *)malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0)
                src_error("Error reading file");

            file->size = fread(file->data, sizeof(char), bufsize, fp);
            if (file->size == 0) 
                src_error("Error reading file %s", file_name);
            else 
                file->data[file->size] = '\0'; /* Just to be safe. */

            src_reset(file);
        }
        fclose(fp);
    }
    else
    {
        src_error("Could not open file: %s", file_name);
    }
    return file;
}

int src_at(SourceFile *file, int index)
{
    int i = file->pos + index;
    if (i >=0 && i < file->size)
        return file->data[i];
    else
        return  -1;
}

int src_next(SourceFile *file)
{
    if (file->pos < file->size)
    {
        file->pos++;
        if (file->pos < file->size)
            file->current = file->data[file->pos];
        else 
            file->current = -1;
    }
    return file->current;
}

int src_next_n(SourceFile *file, int n)
{
    int i;
    for (i = 0; i < n; i++)
        src_next(file);
    return file->current;
}

int src_is_whitespace(SourceFile *file)
{
    int c = file->current;
    return c == ' ' || c == '\n' ||  c == '\t' || c == '\r';
}

int src_is_eof(SourceFile *file)
{
    return file->pos == file->size;
}

int src_next_nonwhite(SourceFile *file)
{
    while (!src_is_eof(file) && src_is_whitespace(file))
        src_next(file);
    return file->current;
}

int src_next_until(SourceFile *file, char expected)
{
    while (!src_is_eof(file) && file->current != expected)
        src_next(file);
    return file->current;
}

char *src_chunk(SourceFile *file, int start, int end)
{
    static char chunk[255];
    int size = end - start;
    strncpy(chunk, file->data + start, size);
    chunk[size + 1] = '\0';
    return chunk;
}

void src_preproc(SourceFile *file)
{
    int c = src_next(file);
    int index = 0;
    List include_list;
    ListElem *e;
    int total = 0;
    int pos = 0;
    char *content = NULL;

    list_init(&include_list);

    while (!src_is_eof(file))
    {
        if (c == '#')
        {
            if (src_at(file, 1) == 'i' &&
                src_at(file, 2) == 'n' &&
                src_at(file, 3) == 'c' &&
                src_at(file, 4) == 'l' &&
                src_at(file, 5) == 'u' &&
                src_at(file, 6) == 'd' &&
                src_at(file, 7) == 'e')
            {
                SourceFile *f;
                char *path;
                int start;
                
                src_next_n(file, 8);
                src_next_nonwhite(file);
                 
                if (file->current != '"')
                    src_error("Expected a file name between \" characters");
                else
                    src_next(file);

                start = file->pos;
                src_next_until(file, '"');
                
                path = src_chunk(file, start, file->pos);
            
                f = src_load(path);
                src_preproc(f);
                total += f->size;
                list_push(&include_list, f);
            }
        }
        else
        {
            file->data[index] = c;
            index++;
        }
        c = src_next(file);
    }
    
    file->size = index;
    file->data[index] = '\0';
    total += index;

    content = (char*)malloc(sizeof(char) * (total * 100));
    e = include_list.first;
    content[0] = '\0'; 
    while (e != NULL)
    {
        SourceFile *f =  (SourceFile*)e->data;
        strcat(content, f->data);
        pos = pos + file->size;
        src_free(f);
        e = e->next;
    } 

    strcat(content, file->data);
    pos = pos + file->size;
    file->size = pos;
    
    free(file->data);
    file->data = content; 
    file->data[total] = '\0';
    src_reset(file);
}


