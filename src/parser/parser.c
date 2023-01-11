#include <stdlib.h>
#include "parser.h"
#include "../exec/exec.h"
#include "../ast/print_ast.h"




/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
int parse(struct lexer *lexer)
{
    //Check for eof or newline
    
    struct parser *parser = malloc(sizeof(struct parser));
    if(!parser)
        return RC_ERROR;
    parser->ast = NULL;
    parser->nodes = ast_list_init();

    int rc = parse_input(lexer, parser);
    
    if(rc == RC_ERROR) //TODO: Free all structures to avoid memory leak. Then return error code.
        return RC_ERROR;

    ast_exec(parser->ast);


    // representing the ast
   // print_ast( parser);
    

   // parser_destroy(parser);

    return RC_SUCCESS;

}

struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{   
    struct ast *ast = parse_pipeline(lexer, parser);
    return ast;
}

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{
    return parse_command(lexer,parser);
}


void parser_destroy(struct parser *parser)
{   
    struct ast *root = parser->ast; 
    struct ast_node *next = NULL;
    struct ast_node *nodes = parser->nodes;
    while (nodes != NULL)
    {
        struct ast *ast = nodes->ast;
        if (nodes->next != NULL)
        {
            next = nodes->next;
        }
        if(ast && ast->node_type == SIMPLE_COMMAND)
        {
            free_ast_simple_command(ast);
        }
        //free(nodes->ast);
        nodes = next;
    
    }
    if (root->node_type == SIMPLE_COMMAND)
    {
         free_ast_simple_command(parser->ast);
    }
    free(parser->nodes);
    free(parser);
}






