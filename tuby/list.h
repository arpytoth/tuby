#ifndef _LIST_H_
#define _LIST_H_

/* An element in the linked list. */
typedef struct list_elem_t
{
    void *data; /* Actual data stored in this list. */
    struct list_elem_t *next; /* Pointer to the next element in the list.*/
} list_elem;

/* Structure that holds the data of a linked list. */
typedef struct list_t
{
    struct list_elem_t *first;
    struct list_elem_t *last;
} list;

/* Initialize a new linked list. */
void list_init(list *l);

/* Release data held by this linked list. */
void list_free(list *l);

/* Push a new element at the end of this list. */
void list_push(list *l, void *e);


#endif // _LIST_H_
