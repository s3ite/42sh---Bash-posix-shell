#include "parser.h"
static struct ast * handle_shell_command(struct lexer *lexer, struct parser *parser)
{
    struct ast *res = NULL;
    struct ast *redir = NULL;

    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_IF)
    {
        res = parse_shell_command(lexer, parser);
        if (!res)
            exit(999);
        // printf("%s\n", "parse_command null");
    }


    // on peek a nouveau dans le lexer
    token = lexer->data[lexer->index+1];
    if (token->type == TOKEN_REDIRECTION)
    {
        // On pop la simple command
        lexer_pop(lexer);
        
        //redir = parse_redirection(lexer, parser, res);

        //if (redir)
        //    return redir;
        (void) redir;
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
    if ((res = handle_shell_command(lexer, parser)) != NULL)
        return res;
    
    // handle simple command
    else if (token->type == WORD)
    {
        res = add_simple_commande(lexer, parser);
        if (!res)
            exit(999);
        // printf("%s\n", "add_simple_commande null");
    }

    

    if (!res)
        exit(999);
    // printf("%s\n", "parse_command  end null");
    return res;
}
