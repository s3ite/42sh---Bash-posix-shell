#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "../parser/parser.h"
#include <string.h>

int my_true(void);
int my_false(void);

void my_exit(struct parser *parser);
void my_unset(void);

int my_echo(char **cmd, size_t arg_number, size_t index);


#endif
