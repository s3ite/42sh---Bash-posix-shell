#ifndef EXEX_H
#define EXEX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "../built_in/built_in.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

int run_command(char **cmd);

#endif /*EXEC_H*/
