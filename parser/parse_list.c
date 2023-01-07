#include "parser.h"
#include <stdlib.h>

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *tmp;
    struct token *peek = NULL;
    while ((peek = lexer_peek(lexer))->type != TOKEN_EOF && peek->type != TOKEN_NEWLINE)
    {
       // printf("%s\n", peek->value);
        tmp = parse_and_or(lexer, parser);
        if (!tmp)
        {
            printf("%s\n", "return null");
            return NULL;
        }
        ast_append(parser->nodes, tmp);
        if (lexer_peek(lexer)->type == TOKEN_EOF)
        {
            break;
        }
        else if (lexer_peek(lexer)->type == TOKEN_SEMICOLON|| lexer_peek(lexer)->type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);
        }
        else
        {
            return NULL;
        }
    }
    return tmp;


}

