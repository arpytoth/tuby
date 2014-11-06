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
