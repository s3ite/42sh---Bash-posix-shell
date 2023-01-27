#ifndef EXPANSION_H
#define EXPANSION_H

#include <ctype.h>
#include <stdbool.h>

#include "../ast/variable.h"

bool contains_expansions(char *input);
char *expansion_handler(char *input, struct variables_list *list);

#endif /* ! EXPANSION_H */
