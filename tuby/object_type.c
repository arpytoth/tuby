#include <stdlib.h>
#include <string.h>

#include "object_type.h"

///////////////////////////////////////////////////////////////////////////////
//                                MEMBER MAP                                  //     
////////////////////////////////////////////////////////////////////////////////

MemberMap *member_map_create()
{
    MemberMap *map = (MemberMap*)malloc(sizeof(MemberMap));
    member_map_init(map);
    return map;
}

//----------------------------------------------------------------------------//

void member_map_destroy(MemberMap *map)
{
   free(map);
}

//----------------------------------------------------------------------------//

void member_map_init(MemberMap *member_map)
{
    member_map->tree = srb_new();
}

//----------------------------------------------------------------------------//
