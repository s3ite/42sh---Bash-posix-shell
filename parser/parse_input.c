#include "header.h"

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
