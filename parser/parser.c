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
    //Check for eof or newline
    
    struct parser *parser = malloc(sizeof(struct parser));
    if(!parser)
        return RC_ERROR;
    parser->ast = NULL;
    parser->node = NULL;

    rc = parse_input(lexer, parser);
    if(rc == RC_ERROR) //TODO: Free all structures to avoid memory leak. Then return error code.
        return RC_ERROR;
    //TODO ...

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


struct ast *parse_command(struct lexer *lexer, struct parser *parser)
{
    return parse_simple_commande(lexer,parser);
}








