#include <stdlib.h>
#include "array.h"
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
