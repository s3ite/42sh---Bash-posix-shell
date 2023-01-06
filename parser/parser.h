#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"

#define RC_ERROR -1
#define RC_SUCCESS 0

struct parser{
    struct ast *ast;
    struct ast_nodes *nodes;
};

int parse(struct lexer *lexer);

int parse_input(struct lexer *lexer, struct parser *parser);
struct ast *parse_list(struct lexer *lexer, struct parser *parser);

struct ast parse_simple_commande(struct lexerlexer, struct parser *parser);
struct ast *parse_if(struct lexer *lexer, struct parser *parser);




#endif /* ! PARSER_H */



