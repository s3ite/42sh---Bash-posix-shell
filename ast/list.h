#ifndef LIST
#define LIST

#include <stddef.h>
#include <stdlib.h>

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

int dlist_push_front(struct dlist *list, char *data);
int dlist_push_back(struct dlist *list, char *data);
size_t dlist_size(const struct dlist *list);
void dlist_print(const struct dlist *list)


#endif /* LIST */