#include "parser.h"
#include <stdlib.h>




/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
int parse(struct lexer *lexer)
{
    struct parser *parser = malloc(sizeof(struct parser));
    if(!parser)
        return RC_ERROR;
    parser->ast = NULL;
    parser->node = NULL;

    struct ast *ast = parse_input(lexer, parser);
    if(!ast) //TODO: Free all structures to avoid memory leak. Then return error code.
        return RC_ERROR;
    //TODO ...

}


int parse_input(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);
    if(!token)
        return RC_SUCCESS;
    struct ast *root = parse_list(lexer, parse);
    if(!root)
        return RC_ERROR;

    parser->ast = root;

    return RC_SUCCESS;

}

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{

    struct ast *and_or_ast = parse_and_or();
   
    if(!and_or_ast)
        return NULL;

    struct token *next_token = lexer_peek(lexer);
    

    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = LIST;
    ast->node = //



}
struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{
    parse_pipeline();
}
struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{

}
struct ast *parse_command(struct lexer *lexer, struct parser *parser)
{

}


/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/
struct ast *parse_simple_commande(struct lexer *lexer, struct parser *parser)
{
	struct simple_command_node *simple_command = malloc(sizeof(struct simple_command_node));
	struct ast *ast = malloc(sizeof(struct ast));
	ast->node_type = SIMPLE_COMMAND;
	ast->node = simple_command;

    // init simple_command_node (str_list) a terminer
	struct ast_node *ast_node = malloc(sizeof(struct ast_node));
    ast_node->ast = ast;
    ast_node->next = NULL;

    return RC_SUCCESS;

}







