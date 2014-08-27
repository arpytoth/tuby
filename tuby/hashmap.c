#include <stdlib.h>

#include "hashmap.h"

struct HashEntry *hashentry_new()
{
    struct HashEntry *entry = NULL;

    entry = (struct HashEntry *)malloc(sizeof(struct HashEntry));
    entry->key = NULL;
    entry->value = NULL;
    entry->next = NULL;
    entry->hash = 0;
}

int hashentry_equals(struct HashEntry *e1, struct HashEntry *e2)
{
}


