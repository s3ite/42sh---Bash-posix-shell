#include <stdlib.h>
#include "parser.h"
#include "../exec/exec.h"




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

    global_exec(parser);

    

    parser_destroy(parser);

    return RC_SUCCESS;

}



struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{
   return parse_pipeline(lexer, parser);
}

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{
    return parse_command(lexer,parser);
}


void parser_destroy(struct parser *parser)
{
    struct ast_node *node = parser->nodes;
   
    while (node != NULL)
    {
        struct ast_node *next = node->next;
        struct ast *ast = node->ast;
        if(ast->node_type == SIMPLE_COMMAND)
        {
            printf("%s\n", "Hit");
            free_ast_simple_command(ast);
        }

        node = next;
    
    }
    free(parser->ast);
    free(parser);
}






