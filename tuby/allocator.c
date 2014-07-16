#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"


Value *alloc_get_val(Value *val)
{
    if (val == NULL)
    {
        val = (Value*)malloc(sizeof(Value));
        val->ref_count = 1;
        printf("Allocated new value: %d\n", (int)val);
    }
    else
    {
        val->ref_count++;
        printf("Increased count to value: %i to %i\n", (int)val, val->ref_count);
    }
    return val;
}


void alloc_free_val(Value *val)
{
    val->ref_count--;
    printf("Decreased count to value: %i to %i\n", (int)val, val->ref_count);
    if (val->ref_count == 0)
    {
        printf("Freeing value %d\n", (int)val);
        free(val);
    }
}


