#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "ast.h"

/*
 * Allocate a new value on the Heap.
 */
Value *alloc_get_val(Value *val);

/*
 * Free value from the Heap, but only if ref_count is zero. 
 */
void alloc_free_val(Value *val);

#endif // _ALLOCATOR_H_

