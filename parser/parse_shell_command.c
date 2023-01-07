#include "parser.h"
#include "../ast/list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// TODO
bool is_shell_command(struct token *token)
{
    //if case
    if(token->type == TOKEN_IF || token->type == TOKEN_THEN ||
     token->type == TOKEN_ELIF || token->type == TOKEN_ELSE || token->type == TOKEN_IF)
    {
        return true;
    }

    return false;
}

void build_node_if(struct token *token, struct ast *node_if)
{
    node_if->type = NODE_IF;
    node_if->node = "if";
}

/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/
struct shell_command_node *parse_shell_commande (struct lexer *lexer,struct parser *parser)
{
		struct token *next_token = lexer_pop(lexer);
	//printf("%s\n", next_token->value);

	if(!is_shell_command(next_token))
		return NULL;

    //struct dlist *compound_list = dlist_init();
    struct condition_if_node *if_condition = malloc(sizeof(struct condition_if_node));
    

}

struct ast *add_shell_commande(struct lexer *lexer, struct parser *parser)
{
    return NULL;
}



void free_shell_command(struct simple_command_node *simple_command) {

}

void free_ast_shell_command(struct ast *ast) {

}