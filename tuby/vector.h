/* A generic vector of pointers implementation. */

#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct
{
    void **data;  /* Contains the data in this vector.*/
    int size;     /* Size of this vector. */
    int reserved; /* Number of reserved items.*/
} vector;

/*
 * Initialize vector.
 */
void vector_init(vector *vector);

/* 
 * Delete vector internal data. This does not release each pointer, you must
 * do this yourself. 
 */
void vector_release(vector *vector);

/* Get element at specified index. */
void *vector_at(vector *v, int index);

/* Get the size of this vector.*/
int vector_length(vector *v);

/* Reserve x size in advance. */
void vector_reserve(vector *v, int size);

/* Push a new element at the end of this vector. */
void vector_push(vector *v, void *elem);

#endif // _VECTOR_H_
