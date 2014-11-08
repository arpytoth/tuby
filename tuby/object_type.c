#include <stdlib.h>
#include <string.h>

#include "tuby.h"

//----------------------------------------------------------------------------//

Member *member_create(char *name, struct ValueType *type)
{
    Member *mbr = (Member*)malloc(sizeof(Member));
    mbr->name = strdup(name);
    mbr->val_type = type;
    return mbr;
}

//----------------------------------------------------------------------------//

void member_destroy(Member *member)
{
    free(member->name);
    free(member);
}

//----------------------------------------------------------------------------//

void array_init(struct Array *arr)
{
    arr->data = (vector*)malloc(sizeof(vector));
    vector_init(arr->data);
    arr->data->ref_count = 1;
}

//----------------------------------------------------------------------------//

void obj_free(Object *obj)
{
    if (obj != NULL)
    {
        obj->data->ref_count--;
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

//----------------------------------------------------------------------------//

void obj_use(Object *obj)
{
    obj->members->ref_count++;
}

//----------------------------------------------------------------------------//

void obj_copy(Object *dest, Object *src)
{
    obj_free(dest);
    obj_use(src);
    dest->members = src->members;
}

//----------------------------------------------------------------------------//


