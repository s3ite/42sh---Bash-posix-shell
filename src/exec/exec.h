#ifndef EXEX_H
#define EXEX_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../ast/ast.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

int run_command(char **cmd);
int global_exec(struct parser *parser);

#endif /*EXEC_H*/
