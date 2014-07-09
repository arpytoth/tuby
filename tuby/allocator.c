#include <stdlib.h>
#include "allocator.h"

Value *get_val(Value *val)
{
    if (val == NULL)
    {
        val = (Value*)malloc(sizeof(Value));
        val->ref_count = 1;
    }
    else
    {
        val->ref_count++;
    }
    return val;
}

void free_val(Value *val)
{
    val->ref_count--;
    if (val->ref_count == 0)
        free(val);
}


