#include "hash_map.h"

#include <stdlib.h>
#include <string.h>

size_t hash(const char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    hash += i;

    return hash;
}

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *hmap = malloc(sizeof(struct hash_map));
    if (!hmap)
        return NULL;
    struct pair_list **data = malloc(sizeof(struct pair_list *) * size);
    if (!data)
        return NULL;
    hmap->data = data;
    hmap->size = size;
    for (size_t i = 0; i < size; i++)
        data[i] = NULL;
    return hmap;
}

bool hash_map_insert(struct hash_map *hash_map, const char *key,
                     struct ast *value)
{
    if (!hash_map || hash_map->size == 0)
        return NULL;
    size_t h = hash(key) % hash_map->size;
    struct pair_list *l = malloc(sizeof(struct pair_list));
    if (!l)
        return false;
    l->key = key;
    l->value = value;
    struct pair_list *tmp = hash_map->data[h];
    while (tmp)
    {
        if (tmp->key == key)
        {
            tmp->value = value;
            free(l);
            return true;
        }
        tmp = tmp->next;
    }
    tmp = hash_map->data[h];
    l->next = tmp;
    hash_map->data[h] = l;
    return true;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (!hash_map)
        printf("%s\n", "not");
    if (!hash_map)
        return;
    size_t i = 0;
    while (i < hash_map->size)
    {
        struct pair_list *l = hash_map->data[i];
        while (l)
        {
            struct pair_list *tmp = l->next;
            free(l);
            l = tmp;
        }
        i++;
    }
    free(hash_map->data);
    free(hash_map);
}

struct ast *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (!hash_map || hash_map->size == 0)
        return NULL;
    size_t h = hash(key) % hash_map->size;
    struct pair_list *tmp = hash_map->data[h];
    while (tmp)
    {
        if (tmp->key && (strcmp(tmp->key, key) == 0))
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (!hash_map || hash_map->size == 0)
        return NULL;
    size_t h = hash(key) % hash_map->size;
    struct pair_list *tmp = hash_map->data[h];
    struct pair_list *before = hash_map->data[h];
    while (tmp)
    {
        if (tmp->key == key)
        {
            if (before == tmp)
            {
                hash_map->data[h] = tmp->next;
                free(tmp);
                return true;
            }
            else
            {
                before->next = tmp->next;
                free(tmp);
                return true;
            }
        }
        before = tmp;
        tmp = tmp->next;
    }
    return false;
}
