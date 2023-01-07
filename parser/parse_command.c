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
