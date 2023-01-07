#include "parser.h"
#include <stdlib.h>

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *tmp;
    struct token *token = NULL;
    struct ast *root = NULL;
    int is_root = 1;

    while ((token = lexer_peek(lexer))->type != TOKEN_EOF && token->type != TOKEN_NEWLINE)
    {
        is_root = 1;
        tmp = parse_and_or(lexer, parser);
        if(!root)
        {
            root = tmp; //Sauvegarde de la tête
            is_root = 0;
        }
        if (!tmp)
            return NULL;

        if(is_root != 0)
            ast_append(parser->nodes, tmp);

        if (lexer_peek(lexer)->type == TOKEN_EOF)
            break;
        else if (lexer_peek(lexer)->type == TOKEN_SEMICOLON || lexer_peek(lexer)->type == TOKEN_NEWLINE)
            lexer_pop(lexer);
        else
            return NULL;
        
    }
    return root;


}

