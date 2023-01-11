#include "parser.h"
#include <stdlib.h>


static struct ast *apply_operator(enum operator_type type, struct ast *left, struct ast *right)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = OPERATOR;
  
    struct operator_node *node = malloc(sizeof(struct operator_node));
    node->type = type;
    node->left = left;
    node->right = right;

    ast->node = node;
    return ast;
}


static struct ast *apply_operator_list(struct ast *res, struct ast *left,struct ast *right, struct token *op)
{
    if (op && op->type == TOKEN_SEMICOLON)
        res = apply_operator(SEMICOLON, left, right);
    return res;
}

struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
   
    struct ast *res = parse_and_or(lexer, parser);
    if (!res)
        return res;
    ast_append(parser->nodes, res);
   
    struct token *token = lexer_peek(lexer);
     while (token && (token->type == TOKEN_SEMICOLON))
     {
        struct token *op = token_init( NULL,token->type);
        struct ast *right = NULL;

        lexer_pop(lexer);

        token = lexer_peek(lexer);
        if (token && token->type != TOKEN_EOF && token->type != TOKEN_NEWLINE)
        {
            right = parse_and_or(lexer, parser);
            if(!right)
                return right;
        }
        res = apply_operator_list(res, res, right, op);
        ast_append(parser->nodes, res);
        token = lexer_peek(lexer);                                                                                                                  
    
    }
    token = lexer_peek(lexer);
    if (!token || (token->type != TOKEN_EOF && token->type != TOKEN_NEWLINE))
        printf("%s\n", "bad terminaison");

    return res;
   
   
  
}

