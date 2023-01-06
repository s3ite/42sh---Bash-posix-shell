#ifndef LIST
#define LIST

#include <stddef.h>

struct dlist
{
    struct dlist_item *head; 
    struct dlist_item *tail;
    size_t size;

};

struct dlist_item 
{
    char *value; 
    struct dlist_item *next; 
    struct dlist_item *prev; 
    int quote;
};

struct dlist *dlist_init(void);
int dlist_push_front(struct dlist *list, int element);
int dlist_push_back(struct dlist *list, int element);
size_t dlist_size(const struct dlist *list);


#endif /* LIST */