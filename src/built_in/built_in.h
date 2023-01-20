#ifndef BUILT_IN_H
#define BUILT_IN_H

#include <string.h>

#include "../parser/parser.h"

int my_true(void);
int my_false(void);

int my_exit(void);
void my_unset(void);

int my_cd(char **cmd);

int my_echo(char **cmd, size_t arg_number, size_t index);

#endif
