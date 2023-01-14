#include "parser.h"
#include <err.h>

struct ast *has_redirection(struct lexer *lexer, struct parser *parser, struct ast *command1)
{
    struct ast *redir = NULL;
    struct token *token = lexer_peek(lexer);

    if (token->type == TOKEN_REDIRECTION)
    {   
        redir = parse_redirection(lexer, parser, command1);
        if (redir)
            return redir;
    }

    return command1;
}

static struct ast * handle_shell_command(struct lexer *lexer, struct parser *parser)
{
    struct ast *res = NULL;

    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_IF || token->type == TOKEN_WHILE)
    {
        res = parse_shell_command(lexer, parser);
        if (!res)
            errx(999, "%s\n", "parse_command null");
    }

    res = has_redirection(lexer, parser, res);
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

    if ((res = handle_shell_command(lexer, parser)) != NULL)
        return res;
    
    // handle simple command
    else if (token->type == WORD)
    {
        res = add_simple_commande(lexer, parser);
        res = has_redirection(lexer, parser, res);
        if (!res)
            errx(999,"add_simple_commande null" );
    }

    

    if (!res)
        errx(999, "parse_command  end null");
    return res;
}
