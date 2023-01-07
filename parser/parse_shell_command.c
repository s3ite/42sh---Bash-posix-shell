#include "parser.h"
#include "../ast/list.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/


struct shell_command_node *parse_shell_commande (struct lexer *lexer,struct parser *parser)
{
	return NULL;
}

struct ast *add_shell_commande(struct lexer *lexer, struct parser *parser)
{
    return NULL;
}



void free_shell_command(struct simple_command_node *simple_command) {

}

void free_ast_shell_command(struct ast *ast) {

}