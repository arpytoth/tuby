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

////////////////////////////////////////////////////////////////////////////////
//                                 OBJECT                                     //
////////////////////////////////////////////////////////////////////////////////

void obj_init(Object *obj)
{
    obj->members = list_create();
    obj->members->ref_count = 1;
}

//----------------------------------------------------------------------------//

void obj_free(Object *obj)
{
    if (obj != NULL)
    {
        obj->members->ref_count--;
        if (obj->members->ref_count == 0)
        {
            ListElem *e = obj->members->first;
            while (e != NULL)
            {
                Var *var = (Var*)e->data;
                var_free(var);
                e = e->next;
            }
            list_free(obj->members);
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


