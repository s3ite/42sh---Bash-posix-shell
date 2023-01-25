#include "parser.h"

#include <stdlib.h>

#include "../ast/print_ast.h"
#include "../exec/exec.h"

struct hash_map *hash_func;


void set_rc(int res, struct global_var *ret)
{
    ret->rc = res;
}

int get_rc(struct global_var *ret)
{
    return ret->rc;
}

struct hash_map *get_functions()
{
    return hash_func;
}
/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
struct parser *parse(struct lexer *lexer)
{
    // Check for eof or newline
    hash_func = hash_map_init(3);
    struct parser *parser = malloc(sizeof(struct parser));
    if (!parser)
        return NULL;

    parser->ast = NULL;
    parser->nodes = ast_list_init();

    int rc = parse_input(lexer, parser);
    if (!rc)
    {
        struct token *token = lexer_peek(lexer);
        if (token->type == TOKEN_EOF)
        {
            fprintf(stderr, "Invalid grammar: GET EOF\n");
            node_free(parser->nodes);
            free(parser);
            return NULL;
        }
        fprintf(stderr, "42sh: syntax error near unexpected token %s\n",
                token->value);
        node_free(parser->nodes);
        free(parser);
        return NULL;
    }
    return parser;
}

static struct ast *build_operator_node(enum operator_type type,
                                       struct ast *left, struct ast *right)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = OPERATOR;
    ast->node = NULL;

    struct operator_node *node = malloc(sizeof(struct operator_node));
    node->type = type;
    node->left = left;
    node->right = right;

    ast->node = node;
    return ast;
}

static int is_condition_token(struct token *token)
{
    if (!token)
        return 0;
    return token->type == TOKEN_AND || token->type == TOKEN_OR;
}

struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = parse_pipeline(lexer, parser);
    if (!ast)
        return ast;
    ast_append(parser->nodes, ast);
    struct token *token = lexer_peek(lexer);

    struct ast *pipe = NULL;

    while (is_condition_token(token))
    {
        if (!token)
            break;

        struct token *copy = token_init(token->value, token->type);

        lexer_pop(lexer);
        token = lexer_peek(lexer);

        if ((pipe = parse_pipeline(lexer, parser)) == NULL)
        {
            token_free(copy);
            return pipe;
        }
        ast_append(parser->nodes, pipe);

        if (copy->type == TOKEN_OR)
        {
            ast = build_operator_node(OR, ast, pipe);
            ast_append(parser->nodes, ast);
        }
        if (copy->type == TOKEN_AND)
        {
            ast = build_operator_node(AND, ast, pipe);
            ast_append(parser->nodes, ast);
        }
        token = lexer_peek(lexer);
        token_free(copy);
    }
    if (!ast)
        return NULL;
    return ast;
}

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{
    int neg = 0;
    struct token *token = lexer_peek(lexer);
    if (token && token->type == TOKEN_NEG)
    {
        lexer_pop(lexer);
        neg = 1;
    }

    struct ast *ast = parse_command(lexer, parser);
    if (!ast)
        return ast;
    ast_append(parser->nodes, ast);

    token = lexer_peek(lexer);
    while (token && token->type == TOKEN_PIPELINE)
    {
        token = lexer_pop(lexer);
        token = lexer_peek(lexer);
        if (!token)
            return ast;
        while (token->type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);
            token = lexer_peek(lexer);
        }

        struct ast *next_cmd = parse_command(lexer, parser);
        if (!next_cmd)
            return next_cmd;
        ast_append(parser->nodes, next_cmd);
        ast = build_operator_node(PIPE, ast, next_cmd);
        ast_append(parser->nodes, ast);

        token = lexer_peek(lexer);
        if (!token)
            break;
    }

    if (neg)
    {
        ast = build_operator_node(NEG, ast, NULL);
        ast_append(parser->nodes, ast);
    }

    return ast;
}

void node_free(struct ast_node *nodes)
{
    if (!nodes)
        return;
    node_free(nodes->next);
    ast_free(nodes->ast);
    free(nodes);
}

void ast_free(struct ast *ast)
{
    if (ast && ast->node_type == SIMPLE_COMMAND)
    {
        free_ast_simple_command(ast);
    }
    if (ast && ast->node_type == SHELL_COMMAND)
    {
        struct shell_command_node *node = ast->node;
        free(node->node);
        free(node);
    }
    if (ast && ast->node_type == OPERATOR)
    {
        struct operator_node *op = ast->node;
        free(op);
    }
    if (ast && ast->node_type == REDIRECTION)
    {
        struct redirection_node *rd_node = ast->node;
        free_ast_redirection(rd_node);
    }
    if (ast)
        free(ast);
}

/**
 * Free the parser structure
 */
void parser_free(struct parser *parser)
{
    // ast_free(parser->ast);
    if (!parser)
        return;
    node_free(parser->nodes);
    free(parser);
}
