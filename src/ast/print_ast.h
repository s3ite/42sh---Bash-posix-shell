#ifndef PRINT_AST
#define PRINT_AST

#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "ast.h"

void print_ast(struct parser *parser);

#endif /* ! PRINT_AST */
