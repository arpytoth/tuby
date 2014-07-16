#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"
#include "log.h"

Value *alloc_get_val(Value *val)
{
    if (val == NULL)
    {
        val = (Value*)malloc(sizeof(Value));
        val->ref_count = 1;
        LOG(llDebug, "iAllocated new value: %d", (int)val);
    }
    else
    {
        val->ref_count++;
        LOG(llDebug, "Increased count to value: %i to %i", 
            (int)val, val->ref_count);
    }
    return val;
}


void alloc_free_val(Value *val)
{
    val->ref_count--;
    LOG(llDebug, "Decreased count to value: %i to %i\n", 
        (int)val, val->ref_count);

    if (val->ref_count == 0)
    {
        LOG(llDebug, "Freeing value %d", (int)val);
        free(val);
    }
}


