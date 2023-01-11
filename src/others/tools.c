#include "tools.h"

char *strdup(const char *str)
{
    size_t len;
    char *copy;

    len = strlen(str) + 1;
    if (!(copy = malloc(len)))
        return (NULL);
    memcpy(copy, str, len);
    return (copy);
}
