#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

typedef enum
{
    llError,
    llDebug
} LogLevel;


#define LOG(LEVEL, MSG, args...)\
if (0)\ 
{\
printf(MSG, ##args);\
printf("\n");\
}
#endif // _LOG_H_
