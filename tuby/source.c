#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

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

typedef struct SrcFileInfo
{
    char *filename;
    int start;
    int end;
    struct SrcFileInfo *next;
    struct SrcFileInfo *prev;
} SrcFileInfo;


SrcFileInfo *src_create_info(const char *filename, int start, int end)
{
    SrcFileInfo *info = (SrcFileInfo*)malloc(sizeof(SrcFileInfo));
    info->start = start;
    info->end = end;
    info->next = NULL;
    info->prev = NULL;
    info->filename = strdup(filename);
    return info;
}

void src_destroy_info(SrcFileInfo *info)
{
    free(info->filename);
    free(info);
}

////////////////////////////////////////////////////////////////////////////////

typedef struct SrcFile
{
    char *name;
    char *data;
    int pos;
    int col;
    int line;
    int size;
    int current;
    SrcFileInfo *info;
} SrcFile;

SrcFileInfo *src_last_info(SrcFile *file)
{
    SrcFileInfo *info = file->info;
    while (info->next != NULL)
    {
        info = info->next;
    }
    return info;
}

void src_append_info(SrcFile *dest, SrcFile *source)
{
    SrcFileInfo *info = src_last_info(dest);
    info->next = source->info; 
}

void src_reset(SrcFile *file)
{
    file->current = -1;
    file->pos = -1;
    file->line = 0;
    file->col = 0;
}

//------------------------------------------------------------------------------

void src_load_from_file(SrcFile *file, const char *file_name)
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

//------------------------------------------------------------------------------

SrcFile *src_create(const char *file_name)
{
    SrcFile *file = (SrcFile*)malloc(sizeof(SrcFile));
    file->name = strdup(file_name);
    src_load_from_file(file, file_name);
    file->info = src_create_info(file_name, 0, file->size - 1);
    return file;
}

//------------------------------------------------------------------------------

void src_free(SrcFile *file)
{
    free(file->name);
    free(file->data);
    free(file);
}

//------------------------------------------------------------------------------

int src_at(SrcFile *file, int index)
{
    int i = file->pos + index;
    if (i >=0 && i < file->size)
        return file->data[i];
    else
        return  -1;
}

int src_next(SrcFile *file)
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

int src_next_n(SrcFile *file, int n)
{
    int i;
    for (i = 0; i < n; i++)
        src_next(file);
    return file->current;
}

int src_is_whitespace(SrcFile *file)
{
    int c = file->current;
    return c == ' ' || c == '\n' ||  c == '\t' || c == '\r';
}

int src_is_eof(SrcFile *file)
{
    return file->pos == file->size;
}

int src_next_nonwhite(SrcFile *file)
{
    while (!src_is_eof(file) && src_is_whitespace(file))
        src_next(file);
    return file->current;
}

int src_next_until(SrcFile *file, char expected)
{
    while (!src_is_eof(file) && file->current != expected)
        src_next(file);
    return file->current;
}

char *src_chunk(SrcFile *file, int start, int end)
{
    static char chunk[255];
    int size = end - start;
    strncpy(chunk, file->data + start, size);
    chunk[size + 1] = '\0';
    return chunk;
}


void src_correct_indices(SrcFile *file, int start)
{
    SrcFileInfo *info = file->info;
    if (start != 0)
        start++;

    while (info != NULL)
    {
        info->start += start;
        info->end += start;
        info = info->next;
    }
}

void src_preproc(SrcFile *file);

void src_process_include(SrcFile *file)
{ 
    SrcFileInfo *last_file_info;
    SrcFile *include_file;
    char *path;
    int start;
    int end_of_first_part;
    int total;
    char *content;
    
    end_of_first_part = file->pos - 1;
    src_next_n(file, 8);
    src_next_nonwhite(file);
    if (file->current != '"')
        src_error("Expected a file name between \" characters");
    else
        src_next(file);

    start = file->pos;
    src_next_until(file, '"');
    path = src_chunk(file, start, file->pos);
    printf("Include file %s\n", path);
    
    src_next(file);

    include_file = src_create(path);
    src_preproc(include_file);

    total = end_of_first_part + 1;
    total = total + include_file->size - 1;
    total = total + file->size - file->pos;
    printf("Total file size is %d\n", total);

    content = (char*)malloc(sizeof(char) * (total + 1));
    strncpy(content, file->data, end_of_first_part + 1);
    content[end_of_first_part + 1] = '\0';
    strcat(content, include_file->data);
    strcat(content, file->data + file->pos + 1);

    free(file->data);
    file->data = content;
    file->size = total;
    file->pos = end_of_first_part + include_file->size;
    file->current = file->data[file->pos];

    src_correct_indices(include_file, end_of_first_part);
    last_file_info = src_last_info(file);
    
        printf("End of first part %d\n", end_of_first_part);
    if (end_of_first_part == 0)
    {
        src_destroy_info(last_file_info);
        file->info = include_file->info;
    }
    else if (last_file_info->start >= end_of_first_part)
    {        
        SrcFileInfo *parent = last_file_info->prev;
        src_destroy_info(last_file_info);
        if (parent == NULL)
            file->info = include_file->info;
        else
            parent->next = include_file->info;
        
        include_file->info->prev = parent;
    }
    else
    {   
        last_file_info->end = end_of_first_part;
        last_file_info->next = include_file->info;
        include_file->info->prev = last_file_info;
    }
    
    last_file_info = src_last_info(file);
    if (total-1 > last_file_info->end + 1)
    { 
        last_file_info->next = src_create_info(file->name, 
            last_file_info->end + 1, total - 1);
        last_file_info->next->prev = last_file_info;
    }
    src_free(include_file);

    file->data[total] = '\0';
    printf("File is:\n");
    printf("%s\n\n", file->data);
    src_reset(file);
}

void src_preproc(SrcFile *file)
{
    int c = src_next(file);
    char *content = NULL;

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
                src_process_include(file);
            }
        }
        c = src_next(file);
    }
}

int main()
{
    int i;
    SrcFileInfo *info;
    SrcFile *file = src_create("p3.txt");
    src_preproc(file);
    printf("THE FINAL FILE IS:\n");
    for (i = 0; i < file->size; i++)
        printf("(%d)%c", file->data[i]);

    info = file->info;
    while (info != NULL)
    {
        printf("INFO: start=%d, stop=%d, file=%s\n", info->start, info->end,
            info->filename);
        info = info->next;
    }
    return 1;
}
