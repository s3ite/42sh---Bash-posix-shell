#include "parser.h"

// Build function node
static struct ast *build_node(struct ast *body)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = FUNCTION;
    ast->node = body;
    return ast;
}

/*
 ** Name: parse_func
 ** Description: parse function rule
 ** struct lexer *lexer, struct parser *parser
 ** Return: struct ast
 */
struct ast *parse_func(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);
    if (token->type != WORD)
        return NULL;
    char *name = token->value;
    if (!name)
        return NULL;

    lexer_pop(lexer); // func () , on pop func
    lexer_pop(lexer); // on pop (

    token = lexer_peek(lexer);
    if (token->type != TOKEN_CLOSE_PAR)
        return NULL; // bad grammar
    lexer_pop(lexer);

    token = lexer_peek(lexer);
    while (token->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        token = lexer_peek(lexer);
    }

    struct ast *body = parse_shell_command(lexer, parser);
    if (!body)
        return NULL;
    ast_append(parser->nodes, body);
    hash_map_insert(get_functions(), name, body);
    return build_node(body);
}
