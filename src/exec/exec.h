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
int exec_w(struct shell_command_node *shell);
int exec_u(struct shell_command_node *shell);

#endif /*EXEC_H*/
