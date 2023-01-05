#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

struct parser{
    struct ast *ast;
    struct ast_nodes *nodes;
};

int parse_input(struct lexer *lexer, struct parser *parser);

int parse_if(struct lexer *lexer, struct parser *parser);




#endif /* ! PARSER_H */



