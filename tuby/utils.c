#include "utils.h"
#include "tokenizer.h"

#include <stdlib.h>
#include <stdio.h>

void error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Error at Line %i, Col %i", g_source.line, g_source.row);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

void parse_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     printf("Parse error at Line %i, Col %i", g_source.line, g_source.row);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}
