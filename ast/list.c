#include "list.h"
#include <stdio.h>
#include <stdlib.h>


struct dlist *dlist_init(void)
{
    struct dlist *head = malloc(sizeof(struct dlist));
    head->size = 0;
    head->head = NULL;
    head->tail = NULL;

    return head;
}

int dlist_push_front(struct dlist *list, char* data)
{
    if (!data)
        return 0;

    struct dlist_item *newhead = malloc(sizeof(struct dlist_item));
    if (!newhead)
        return 0;

    newhead->value = data;
    newhead->prev = NULL;

    if (!list->head)
    {
        newhead->next = NULL;

        list->head = newhead;
        list->tail = newhead;
        list->size = dlist_size(list);
        return 1;
    }
    newhead->next = list->head;

    list->head->prev = newhead;
    list->head = newhead;

    list->size = dlist_size(list);
    return 1;
}


int dlist_push_back(struct dlist *list, char *data)
{
    if (!data)
        return 0;

    struct dlist_item *newlast = malloc(sizeof(struct dlist_item));
    if (!newlast)
        return 0;

    newlast->value = data;
    newlast->next = NULL;

    if (!list->head)
    {
        newlast->prev = NULL;

        list->head = newlast;
        list->tail = newlast;
        list->size = dlist_size(list);
        return 1;
    }

    newlast->prev = list->tail;

    list->tail->next = newlast;
    list->tail = newlast;

    list->size = dlist_size(list);
    return 1;
}


size_t dlist_size(const struct dlist *list)
{
    if (!list)
        return 0;

    struct dlist_item *head = list->head;
    int res = 0;

    for (; head; head = head->next)
        res++;

    free(head);
    return res;
}


void dlist_print(const struct dlist *list)
{
    if (list->size == 0)
        return;

    struct dlist_item *index = list->head;

    while (index)
    {
        printf("%s\n", index->value);
        index = index->next;
    }
}
