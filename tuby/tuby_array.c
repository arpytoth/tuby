#include <stdlib.h>
#include "tuby_array.h"
#include "vector.h"
#include "allocator.h"

void array_init(struct Array *arr)
{
    arr->data = (vector*)malloc(sizeof(vector));
    vector_init(arr->data);
    arr->data->ref_count = 1;
}

void array_free(struct Array *arr)
{
    if (arr != NULL && arr->data != NULL)
    {
        arr->data->ref_count--;
        if (arr->data->ref_count == 0)
        {
            int i;
            int length = vector_length(arr->data);
            for (i = 0; i < length; i++)
            {
                Value *elem = (Value*)vector_at(arr->data, i);
                if (elem != NULL)
                    alloc_free_val(elem);
            }
            free(arr->data);
            arr->data = NULL;
        }              
    }
}

void array_use(struct Array *arr)
{
    arr->data->ref_count++;
}


void array_copy(struct Array *dest, struct Array *src)
{
    array_free(dest);
    array_use(src);
    dest->data = src->data;
}
