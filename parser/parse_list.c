#include "parser.h"
#include <stdlib.h>

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
    //While != eof & newline
    //peek then If eof break
    //pop if semi or newline
    struct ast *and_or_ast = parse_and_or();  
    if(!and_or_ast)
        return NULL;

    ast_append(parser->to_free, res);

    struct token *next_token = lexer_peek(lexer);
    
    if(!next_token)
        return NULL;

    return and_or_ast;

}

