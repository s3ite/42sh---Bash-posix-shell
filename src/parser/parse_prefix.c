#include "parser.h"

struct ast *parser_prefix(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_REDIRECTION)
    {
        return parse_redirection(lexer, parser);
    }
    return NULL;
}
