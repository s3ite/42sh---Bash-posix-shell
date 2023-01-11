#ifndef PRINT_AST
#define PRINT_AST

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

void print_ast(struct parser *parser);

#endif   /* ! PRINT_AST */
