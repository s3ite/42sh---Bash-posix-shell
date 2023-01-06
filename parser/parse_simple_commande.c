#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/


struct simple_command_node parse_simple_commande (struct lexer *lexer)
{
	struct token *next_token = lexer_pop(lexer);
	if next_token->type != WORD
		//ERROR

	//revoir limplemntation de la liste pour placer le premier token
	struct str_list prefix->head->value = next_token->value;
	next_token = lexer_pop;


	//Si 1er toker est un args -> ajouter dans prefix
	if (next_token->value[0] == '-')
	{
		prefix->head->next->value = next_token->value
	}


	//sinon creer une list value : 
	struct str_list value->head->value = next_token->value;
	while ((next_token = lexer_pop())->type == WORD)
    {
    	value->head->next->value = next_token->value;

   	}
    if (next_token->type != EOF)
        //ERROR
	
	struct simple_command_node *simple_command = malloc(sizeof
		(struct simple_command_node));
	simple_command->prefix = prefix;
	simple_command->values = values;

	return simple_command;

}

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser)
{
	
	struct ast *ast = malloc(sizeof(struct ast));
	ast->node_type = SIMPLE_COMMAND;
	ast->node = simple_command;

    // init simple_command_node (str_list) a terminer
	struct ast_node *ast_node = malloc(sizeof(struct ast_node));
    ast_node->ast = ast;
    ast_node->next = NULL;

    return RC_SUCCESS;

}