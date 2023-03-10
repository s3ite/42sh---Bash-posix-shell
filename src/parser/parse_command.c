#include <err.h>

#include "parser.h"

/*
 ** Name: handle_shell_command
 ** Description: check if expected token is good
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
static struct ast *handle_shell_command(struct lexer *lexer,
                                        struct parser *parser)
{
    struct ast *res = NULL;

    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_IF || token->type == TOKEN_WHILE
        || token->type == TOKEN_UNTIL || token->type == TOKEN_OPEN_ACC
        || token->type == TOKEN_OPEN_PAR)
    {
        res = parse_shell_command(lexer, parser);
    }
    return res;
}

/*
 ** Name: parse_command
 ** Description: parse command
 ** @struct lexer; struct parser
 ** Return: future shell command ast
 */
struct ast *parse_command(struct lexer *lexer, struct parser *parser)
{
    struct ast *res = NULL;
    struct token *token = lexer_peek(lexer);
    struct token *token_two = lexer_peek(lexer);

    if (token->type == WORD
        && (token_two = lexer_peek_two(lexer))->type == TOKEN_OPEN_PAR)
    {
        res = parse_func(lexer, parser);
        ast_append(parser->nodes, res);
    }
    else if ((res = handle_shell_command(lexer, parser)) != NULL)
        return res;

    else if (token->type == WORD)
    {
        res = add_simple_commande(lexer, parser);
    }

    // Redirection des parses commands
    // lexer_pop(lexer);
    token = lexer_peek(lexer);

    if (token->type == TOKEN_EOF || token->type == TOKEN_SEMICOLON)
        return res;

    while (token->type == TOKEN_REDIRECTION)
    {
        res = parse_redirection(lexer, parser);
        if (!res)
        {
            fprintf(stderr, "Error parse_command");
            return NULL;
        }
        ast_append(parser->nodes, res);
        token = lexer_peek(lexer);
    }

    if (!res)
        fprintf(stderr, "parse_command null\n");
    return res;
}
