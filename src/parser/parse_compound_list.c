#include "parser.h"


struct ast *parse_start(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = parse_and_or(lexer, parser);
    if(!ast)
        return ast;

    ast_append(parser->nodes, ast);
    return ast;
}

static struct ast *build_operator_node(enum operator_type type, struct ast *left, struct ast *right)
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


static struct ast *apply_operator_compound(struct ast *res, struct ast *left, struct ast *right, struct token *op)
{
    if (op && (op->type == TOKEN_SEMICOLON || op->type == TOKEN_NEWLINE))
        res = build_operator_node(SEMICOLON, left, right);

    token_free(op);
    return res;
}

static int is_at_end(struct token *token)
{
    enum TokenType type = token->type;
    int rc = type == TOKEN_THEN || type == TOKEN_ELIF || type == TOKEN_ELSE || type == TOKEN_FI;

    return rc;
}


static struct token *consume(struct lexer *lexer)
{
    struct token *token = lexer_peek(lexer);
    struct token *copy = token_init(token->value, token->type);
    lexer_pop(lexer);
    return copy;
}

struct ast *parse_compound_list(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = parse_start(lexer, parser);
    if(!ast)
        return ast;

    struct ast *list = NULL;
    struct token *token = lexer_peek(lexer);
    while(!is_at_end(token))
    {
        if (token->type != TOKEN_SEMICOLON && token->type != TOKEN_NEWLINE)
            return NULL;

        struct token *copy = consume(lexer);
        token = lexer_peek(lexer);
        if(is_at_end(token))
        {
            ast = apply_operator_compound(ast,ast, NULL, copy);
            break;
        }
        list = parse_and_or(lexer, parser);
        if(!list)
        {
            token_free(copy);
            return list;
        }
        ast_append(parser->nodes, list);
        ast = apply_operator_compound(ast, ast, list, copy);
        ast_append(parser->nodes, ast);

    }
    
    return ast;
}
