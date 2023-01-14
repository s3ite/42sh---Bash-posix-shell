#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/list.h"
#include "parser.h"

struct ast *build_shell_command_node(enum shell_type type)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = SHELL_COMMAND;

    struct shell_command_node *node = malloc(sizeof(struct shell_command_node));
    node->type = type;
    node->node = NULL;

    ast->node = node;

    return ast;
}

static struct ast *parse_cmd(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = NULL;
    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_IF)
    {
        ast = build_shell_command_node(IF);
        ast_append(parser->nodes, ast);
        ast = parse_rule_if(lexer, parser, ast);
    }
    else if (token->type == TOKEN_WHILE || token->type == TOKEN_UNTIL)
    {
        ast = build_shell_command_node(WU);
        ast_append(parser->nodes, ast);
        ast = parse_rule_wu(lexer, parser, ast);
    }

    return ast;
}

/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 *
 * Goal : Parse a simple command following the grammar, create a simple command
 * node
 */
struct ast *parse_shell_command(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = NULL;
    ast = parse_cmd(lexer, parser);
    return ast;
}
