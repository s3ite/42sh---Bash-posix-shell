#include "parser.h"

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
