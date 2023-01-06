#include "parser.h"
#include <stdlib.h>

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
    //While != eof & newline
    //peek then If eof break
    //pop if semi or newline
    struct token *token = lexer_peek(lexer);
    struct ast *and_or_ast = parse_and_or();  

    while(token && token->type != TOKEN_EOF && token->type != TOKEN_NEWLINE )
    {
         and_or_ast = parse_and_or();  
        if(!and_or_ast)
            return NULL;

        token = lexer_peek(lexer);
        if(token->type == TOKEN_EOF)
            break;

        if(token->type == TOKEN_SEMICOLON ||token->type == TOKEN_NEWLINE)
            lexer_pop(lexer);

       
        ast_append(parser->nodes, and_or_ast);

        token = lexer_peek(lexer);

    }
    return and_or_ast;

}

