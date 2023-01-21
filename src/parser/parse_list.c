#include <stdlib.h>

#include "parser.h"

static struct ast *build_operator_node(enum operator_type type,
                                       struct ast *left, struct ast *right)
{
    struct ast *ast = init_ast();
    ast->node_type = OPERATOR;

    struct operator_node *node = malloc(sizeof(struct operator_node));
    node->type = type;
    node->left = left;
    node->right = right;

    ast->node = node;
    return ast;
}

static int is_expected_token(struct lexer *lexer)
{
    struct token *token = lexer_peek(lexer);
    if(!token)
        return 0;
    return (token->type == TOKEN_NEWLINE || token->type == TOKEN_EOF);
}

/*
 ** Name: parse_list
 ** Description: parse list rule
 ** Param: @struct lexer * @struct parser *parser
 ** Return: builded ast
 */
struct ast *parse_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *res = parse_and_or(lexer, parser);
    if (!res)
        return res;
    ast_append(parser->nodes, res);

    struct token *token = lexer_peek(lexer);
    while (token && (token->type == TOKEN_SEMICOLON))
    {
        if (!token)
            break;
        struct token *copy = token_init(NULL, token->type);
        struct ast *right = NULL;

        lexer_pop(lexer);

        token = lexer_peek(lexer);
        if (token && token->type != TOKEN_EOF && token->type != TOKEN_NEWLINE
            && ((right = parse_and_or(lexer, parser)) == NULL))
        {
            token_free(copy);
            return right;
        }
        if (copy && copy->type == TOKEN_SEMICOLON)
            res = build_operator_node(SEMICOLON, res, right);

        ast_append(parser->nodes, res);
        token = lexer_peek(lexer);
        token_free(copy);
    }

    if(!is_expected_token(lexer))
        return NULL;
    return res;
}
