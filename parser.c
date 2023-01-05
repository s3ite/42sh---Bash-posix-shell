#include "parser.h"
#include <stdlib.h>

#define RC_SUCCESS 0


int parse_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = LIST;
    ast->node = //

    parse_and_or();


}
int parse_and_or(struct lexer *lexer, struct parser *parser)
{
    parse_pipeline();
}
int parse_pipeline(struct lexer *lexer, struct parser *parser)
{

}
int parse_command(struct lexer *lexer, struct parser *parser)
{

}


/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/
int parse_simple_commande(struct lexer *lexer, struct parser *parser)
{
	struct simple_command_node *simple_command = malloc(sizeof
			(struct simple_command_node));
	struct ast *ast = malloc(sizeof(struct ast));
	ast->node_type = SIMPLE_COMMAND;
	ast->node = simple_command;

    // init simple_command_node (str_list) a terminer
	struct ast_node *ast_node = malloc(sizeof(struct ast_node));
    ast_node->ast = ast;
    ast_node->next = NULL;

    return RC_SUCCESS;

}







