#ifndef _TUBY_SOURCE_H_
#define _TUBY_SOURCE_H_

typedef struct SrcFileInfo
{
    char *filename;
    int start;
    int end;
    struct SrcFileInfo *next;
    struct SrcFileInfo *prev;
} SrcFileInfo;

SrcFileInfo *src_create_info(const char *filename, int start, int end);
void src_destroy_info(SrcFileInfo *info);


typedef struct SrcFile
{
    char *name;
    char *data;
    int pos;
    int col;
    int line;
    int size;
    int current;
    SrcFileInfo *current_file;
    SrcFileInfo *info;
} SrcFile;


SrcFileInfo *src_last_info(SrcFile *file);
void src_reset(SrcFile *file);
void src_load_from_file(SrcFile *file, const char *file_name);
SrcFile *src_create(const char *file_name);
void src_free(SrcFile *file);
int src_at(SrcFile *file, int index);
int src_next(SrcFile *file);
int src_next_n(SrcFile *file, int n);
int src_is_whitespace(SrcFile *file);
int src_is_eof(SrcFile *file);
int src_next_nonwhite(SrcFile *file);
int src_next_until(SrcFile *file, char expected);
char *src_chunk(SrcFile *file, int start, int end);
void src_correct_indices(SrcFile *file, int start);
void src_process_include(SrcFile *file);
void src_preproc(SrcFile *file);

////////////////////////////////////////////////////////////////////////////////

extern SrcFile *g_source;

#endif // _TUBY_SOURCE_H_

