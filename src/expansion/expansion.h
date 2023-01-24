#define _POSIX_C_SOURCE 2

#ifndef EXPANSION_H
#    define EXPANSION_H

#    include <ctype.h>
#    include <stdbool.h>

#    include "../ast/variable.h"

char *expand_variable(char *input, struct variables_list *list);
bool contains_variable(char *input);

#endif /* ! EXPANSION_H */
