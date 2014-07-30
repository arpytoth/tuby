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

