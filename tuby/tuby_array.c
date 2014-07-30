#include <stdlib.h>
#include "tuby_array.h"
#include "vector.h"
#include "allocator.h"

Value *array_get_val(Value *a, int index)
{
    Value *val = NULL;
    vector_resize(&a->data.vector_val, index + 1);
    
    val = vector_at(&a->data.vector_val, index);
    if (val == NULL)
    {
        val = alloc_val(a->value_type->uval_type);
        vector_set_at(&a->data.vector_val, index, val);
    }
    return val;
}

Value *array_set_val(Value *a, int index, Value *val_to_set)
{
    Value *val = NULL;
    vector_resize(&a->data.vector_val, index + 1);

    val = vector_at(&a->data.vector_val, index);
    vector_set_at(&a->data.vector_val, index, val_to_set);
    return val;
}

void array_free(struct Array *arr)
{
    if (arr != NULL && arr->data != NULL)
    {
        arr->data->ref_count--;
        if (arr->data->ref_count == 0)
        {
            int i;
            int length = vector_length(&val->data.vector_val);
            for (i = 0; i < length; i++)
            {
                Value *elem = (Value*)vector_at(&val->data.vector_val, i);
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
    
}
