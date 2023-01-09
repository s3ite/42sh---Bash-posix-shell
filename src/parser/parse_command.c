#include "parser.h"
/*
 ** Name: parse_command
 ** Description: parse command
 ** @struct lexer; struct parser
 ** Return: future shell command ast 
 */
struct ast *parse_command(struct lexer *lexer, struct parser *parser)
{
    struct ast *res = add_simple_commande(lexer,parser);
    return res;

}


struct ast *parse_shell_command(struct lexer *lexer, struct parser *parser)
{
    (void) lexer;
    (void) parser;

    return NULL;
    //struct ast *res = add_shell_commande(lexer,parser);
    return NULL;
}
