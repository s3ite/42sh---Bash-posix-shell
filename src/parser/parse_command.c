#include "parser.h"

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
    if (token->type == TOKEN_IF)
    {
        res = parse_shell_command(lexer, parser);
        if (!res)
            exit(999);
    }
    else if (token->type == TOKEN_WHILE || token->type == TOKEN_UNTIL)
    {
        res = parse_shell_command(lexer, parser);
        if (!res)
            exit(999);
    }
    else if (token->type == WORD)
    {
        res = add_simple_commande(lexer, parser);
        if (!res)
            exit(999);
    }

    if (!res)
        exit(999);
    return res;
}
