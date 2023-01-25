#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

#include "../ast/ast.h"

struct pair_list
{
    const char *key;
    struct ast *value;
    struct pair_list *next;
};

struct hash_map
{
    struct pair_list **data;
    size_t size;
};

size_t hash(const char *str);

struct hash_map *hash_map_init(size_t size);
void hash_map_free(struct hash_map *hash_map);

bool hash_map_insert(struct hash_map *hash_map, const char *key,
                     struct ast *value);
void hash_map_dump(struct hash_map *hash);
struct ast *hash_map_get(const struct hash_map *hash, const char *key);
bool hash_map_remove(struct hash_map *hash, const char *key);

#endif /* !HASH_MAP_H */
