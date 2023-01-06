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


struct simple_command_node *parse_simple_commande (struct lexer *lexer,struct parser *parser)
{
	struct token *next_token = lexer_pop(lexer);
	if (next_token->type != WORD)
		return NULL;

	//revoir limplemntation de la liste pour placer le premier token
	struct dlist *prefix = dlsit_init();
	struct dlist *values = dlist_init();

	dlist_push_front(prefix,next_token->value);

	next_token = lexer_pop;


	//Si 1er toker est un args -> ajouter dans prefix
	if (next_token->value[0] == '-')
	{
		dlist_push_front(prefix,next_token->value);
	}


	//sinon creer une list value : 

    {
    	dlist_push_front(values,next_token->value);
		lexer_pop(lexer);

   	}
    if (next_token->type != EOF)
        return NULL;
	
	struct simple_command_node *simple_command = malloc(sizeof
		(struct simple_command_node));
	simple_command->prefix = prefix;
	simple_command->values = values;

	return simple_command;

}

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser)
{
	struct simple_command_node *node = parse_simple_commande(lexer,parser);
	struct ast *ast = malloc(sizeof(struct ast));
	ast->node_type = SIMPLE_COMMAND;
	ast->node = node;

	return node;
}