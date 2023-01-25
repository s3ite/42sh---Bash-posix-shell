#include "parser.h"



/*
 ** Name: parse_start
 ** Description: restart parsing from start
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
struct ast *parse_start(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);
    while (token->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        token = lexer_peek(lexer);
    }
    struct ast *ast = parse_and_or(lexer, parser);
    if (!ast)
        return ast;

    ast_append(parser->nodes, ast);
    return ast;
}


/*
 ** Name: build_operator_node
 ** Description: build operator node
 ** enum operator_type type,struct ast *left, struct ast *right
 ** Return: struct ast
 */
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


/*
 ** Name: apply_operator_compound
 ** Description: build operator node
 ** struct ast *res, struct ast *left, struct ast *right, struct token *o
 ** Return: struct ast
 */
static struct ast *apply_operator_compound(struct ast *res, struct ast *left,
                                           struct ast *right, struct token *op)
{
    if (op && (op->type == TOKEN_SEMICOLON || op->type == TOKEN_NEWLINE))
        res = build_operator_node(SEMICOLON, left, right);

    token_free(op);
    return res;
}

/*
 ** Name: clone
 ** Description: clone current token
 ** struct lexer *lexer
 ** Return: struct token
 */
static struct token *clone(struct lexer *lexer)
{
    struct token *token = lexer_peek(lexer);
    struct token *copy = token_init(token->value, token->type);
    lexer_pop(lexer);
    token = lexer_peek(lexer);
    while (token->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        token = lexer_peek(lexer);
    }
    return copy;
}


/*
 ** Name: parse_compound_list
 ** Description: parse compound list rule
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
struct ast *parse_compound_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = parse_start(lexer, parser);
    if (!ast)
        return ast;

    struct ast *list = NULL;
    struct token *token = lexer_peek(lexer);

    while (token->type != TOKEN_THEN && token->type != TOKEN_ELIF
           && token->type != TOKEN_ELSE && token->type != TOKEN_FI
           && token->type != TOKEN_DO && token->type != TOKEN_DONE
           && token->type != TOKEN_CLOSE_ACC && token->type != TOKEN_CLOSE_PAR)
    {
        if (token->type != TOKEN_SEMICOLON && token->type != TOKEN_NEWLINE)
            return NULL;

        struct token *copy = clone(lexer);

        token = lexer_peek(lexer);
        if (token->type == TOKEN_THEN || token->type == TOKEN_ELIF
            || token->type == TOKEN_ELSE || token->type == TOKEN_FI
            || token->type == TOKEN_DO || token->type == TOKEN_DONE
            || token->type == TOKEN_CLOSE_ACC || token->type == TOKEN_CLOSE_PAR)
        {
            ast = apply_operator_compound(ast, ast, NULL, copy);
            break;
        }
        list = parse_and_or(lexer, parser);
        if (!list)
        {
            token_free(copy);
            return list;
        }
        ast_append(parser->nodes, list);
        ast = apply_operator_compound(ast, ast, list, copy);
        ast_append(parser->nodes, ast);
        token = lexer_peek(lexer);
    }

    return ast;
}
