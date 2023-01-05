#include "parser.h"


int parse_list(struct lexer *lexer, struct parser *parser)
{

}
int parse_and_or(struct lexer *lexer, struct parser *parser)
{

}
int parse_pipeline(struct lexer *lexer, struct parser *parser)
{

}
int parse_command(struct lexer *lexer, struct parser *parser)
{

}
int simple_commande(struct lexer *lexer, struct parser *parser)
{
	struct simple_command_node *simple_commande = malloc(sizeof
			(struct simple_commande_node));
	struct ast *ast = malloc(sizeof(struct ast));
	ast->node_type = SIMPLE_COMMAND;
	ast->node = simple_commande;

	struct ast_nodes *ast_nodes = malloc(sizeof())

}







