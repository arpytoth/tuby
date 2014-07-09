#include <stdlib.h>
#include "vector.h"

void vector_init(vector *v)
{
    v->data = 0;
    v->reserved = 0;
    v->size = 0;
}


void vector_release(vector *v)
{
    free(v->data);
}


void *vector_at(vector *v, int index)
{
    return v->data[index];
}


int vector_length(vector *v)
{
    return v->size;
}

void vector_reserve(vector *v, int size)
{
    if (size > v->reserved)
    {
        int size_of_pointer = sizeof(void*);
        int new_size = v->reserved == 0 ? 1 : v->reserved * 2;
        v->data = (void**)realloc(v->data, new_size * size_of_pointer);
        v->reserved = new_size;
    }
}


void vector_push(vector *v, void *elem)
{
    int size_of_pointer = sizeof(void*);
    if (v->reserved - v->size == 0)
    {
        int new_size = v->reserved == 0 ? 1 : v->reserved * 2;
        v->data = (void**)realloc(v->data, new_size * size_of_pointer);
        v->reserved = new_size;
    }
    v->size++;
    v->data[v->size - 1] = elem;
}
