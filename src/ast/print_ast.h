#ifndef PRINT_AST
#define PRINT_AST

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

void print_ast(struct ast_node *head);

char *print_simplecommand(struct simple_command_node *simple_command);
char *print_simplecommand_args(struct simple_command_node *simple_command);
#endif   /* ! PRINT_AST */
