#include "utils.h"
#include "tokenizer.h"

#include <stdlib.h>
#include <stdio.h>

void error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}

void parse_error(const char *format, ...)
{
     va_list arguments;  
     va_start(arguments, format);
     vprintf(format, arguments);
     printf("\n");
     va_end(arguments);
     exit(1);
}
