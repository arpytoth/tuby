#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdarg.h>

/* Report an error and exit application. */
void error(const char *format, ...);

void parse_error(const char *format, ...);

#endif // _UTILS_H_
