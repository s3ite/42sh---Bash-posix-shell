#include "parser.h"

struct ast *parse_condition(struct lexer *lexer, struct parser *parser,
                            enum TokenType type)
{
    // struct token *token = lexer_peek(lexer);
    if (!type)
        return NULL;

    lexer_pop(lexer);
    struct ast *ast = parse_compound_list(lexer, parser);
    if (!ast)
        printf("%s\n", "empty");
    if (!ast)
        return ast;
    ast_append(parser->nodes, ast);
    return ast;
}

struct condition_if_node *build_condition_if_node(struct ast *condition,
                                                  struct ast *then_action,
                                                  struct ast *else_action)
{
    struct condition_if_node *ast = malloc(sizeof(struct condition_if_node));

    ast->condition_c = condition;
    ast->then_action = then_action;
    ast->else_action = else_action;

    return ast;
}

struct ast *parse_rule_if(struct lexer *lexer, struct parser *parser,
                          struct ast *prev_ast)
{
    struct ast *cond = parse_condition(lexer, parser, TOKEN_IF);

    if (!cond)
        printf("%s\n", "parse_rule_f parse condition null");
    if (!cond)
        return cond;

    struct ast *right = parse_condition(lexer, parser, TOKEN_THEN);
    if (!right)
        printf("%s\n", "parse_rule_f right null");
    if (!right)
        return right;

    struct ast *else_clause = NULL;
    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_ELSE || token->type == TOKEN_ELIF)
    {
        else_clause = parse_rule_else(lexer, parser);
        if (!else_clause)
            printf("%s\n", "parse_rule_f else_clause null");
        if (!else_clause)
            return else_clause;
        ast_append(parser->nodes, else_clause);
    }

    token = lexer_peek(lexer);
    if (token->type != TOKEN_FI)
        printf("%s got %ud\n", "parse_rule_f oken->type != TOKEN_FI",
               token->type);
    if (token->type != TOKEN_FI)
        return NULL;
    lexer_pop(lexer);

    struct condition_if_node *if_node =
        build_condition_if_node(cond, right, else_clause);
    struct shell_command_node *shell_node = prev_ast->node;
    shell_node->node = if_node;

    return prev_ast;
}
