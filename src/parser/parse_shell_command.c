#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/list.h"
#include "parser.h"

/*
 ** Name: build_shell_command_node
 ** Description: build ast structure for shell command
 ** enum shell_type
 ** Return: struct ast
 */
struct ast *build_shell_command_node(enum shell_type type)
{
    struct ast *ast = init_ast();
    ast->node_type = SHELL_COMMAND;

    struct shell_command_node *node = malloc(sizeof(struct shell_command_node));
    node->type = type;
    node->node = NULL;

    ast->node = node;

    return ast;
}

/*
 ** Name: parse_cmd
 ** Description: parse command rule
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
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

/*
 ** Name: parse_cmd
 ** Description: parse commands block
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
static struct ast *parse_block_cmd(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = build_shell_command_node(BLOCK);
    ast_append(parser->nodes, ast);
    struct token *token = lexer_peek(lexer);
    struct block_node *block = malloc(sizeof(struct block_node));

    if (token->type == TOKEN_OPEN_ACC)
        block->type = TOKEN_OPEN_ACC;
    else if (token->type == TOKEN_OPEN_PAR)
        block->type = TOKEN_OPEN_PAR;
    else
    {
        free(block);
        return NULL;
    }

    token = lexer_pop(lexer);
    struct ast *compound = parse_compound_list(lexer, parser);
    if (!compound)
    {
        free(block);
        return NULL;
    }
    token = lexer_peek(lexer);
    ast_append(parser->nodes, compound);
    block->ast = compound;
    struct shell_command_node *cmd = ast->node;
    cmd->node = block;

    if (block->type == TOKEN_OPEN_ACC && token->type != TOKEN_CLOSE_ACC)
        return NULL;
    if (block->type == TOKEN_OPEN_PAR && token->type != TOKEN_CLOSE_PAR)
        return NULL;
    token = lexer_pop(lexer);
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
    struct token *token = lexer_peek(lexer);
    if (!token)
        return NULL;
    if (token->type != TOKEN_OPEN_ACC && token->type != TOKEN_OPEN_PAR)
        ast = parse_cmd(lexer, parser);
    else
    {
        ast = parse_block_cmd(lexer, parser);
    }
    return ast;
}
