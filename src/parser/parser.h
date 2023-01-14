#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../ast/list.h"
#include "../lexer/lexer.h"
#include "../others/tools.h"

#define RC_ERROR -1
#define RC_SUCCESS 0

struct parser
{
    struct ast *ast;
    struct ast_node *nodes;
};

int parse(struct lexer *lexer);

int parse_input(struct lexer *lexer, struct parser *parser);

struct ast *parse_list(struct lexer *lexer, struct parser *parser);
struct ast *parse_and_or(struct lexer *lexer, struct parser *parser);
struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser);

void node_free(struct ast_node *nodes);
void ast_free(struct ast *ast);
void parser_free(struct parser *parser);

struct ast *build_shell_command_node(enum shell_type type);

struct ast *parse_command(struct lexer *lexer, struct parser *parser);

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser);
struct simple_command_node *parse_simple_commande(struct lexer *lexer,
                                                  struct parser *parser);
struct ast *parse_shell_command(struct lexer *lexer, struct parser *parser);

struct ast *parse_compound_list(struct lexer *lexer, struct parser *parser);

struct ast *parse_rule_if(struct lexer *lexer, struct parser *parser,
                          struct ast *ast);
struct ast *parse_rule_else(struct lexer *lexer, struct parser *parser);
struct ast *parse_rule_elif(struct lexer *lexer, struct parser *parser,
                            struct ast *prev_ast);
struct condition_if_node *build_condition_if_node(struct ast *condition,
                                                  struct ast *then_action,
                                                  struct ast *else_action);

void free_ast_simple_command(struct ast *ast);
void free_simple_command(struct simple_command_node *simple_command);

#endif /* ! PARSER_H */
