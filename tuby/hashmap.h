#ifndef _TUBY_HASHMAP_H_
#define _TUBY_HASHMAP_H_

struct HashEntry
{
    char *key;
    void *value;
    struct HashEntry *next;
    int hash;
}

struct HashEntry *hashentry_new();
int hashentry_equals(struct HashEntry *e1, struct HashEntry *e2);


struct HashMap
{
};

HashMap *hashmap_new();



#endif //_TUBY_HASHMAP_H_
