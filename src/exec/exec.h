#ifndef EXEX_H
#define EXEX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include "../ast/ast.h"
#include "../built_in/built_in.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
//#include "../redirection/redirection.h"

int run_command(char **cmd);
int ast_exec(struct ast *node);
int exec_wu(struct shell_command_node *shell);
int exec_w(struct shell_command_node *shell);
int exec_u(struct shell_command_node *shell);

#endif /*EXEC_H*/
