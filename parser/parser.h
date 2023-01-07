#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../ast/list.h"

#define RC_ERROR -1
#define RC_SUCCESS 0

struct parser{
    struct ast *ast;
    struct ast_node *nodes;
};

int parse(struct lexer *lexer);

int parse_input(struct lexer *lexer, struct parser *parser);

struct ast *parse_list(struct lexer *lexer, struct parser *parser);
struct ast *parse_and_or(struct lexer *lexer, struct parser *parser);
struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser);


struct simple_command_node *parse_simple_commande(struct lexer *lexer,  struct parser *parser);
struct ast *parse_command(struct lexer *lexer, struct parser *parser);

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser);
struct ast *parse_if(struct lexer *lexer, struct parser *parser);





#endif /* ! PARSER_H */



