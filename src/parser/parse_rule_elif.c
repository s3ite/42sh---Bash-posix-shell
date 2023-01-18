#include "parser.h"

/*
 ** Name: parse_rule_elif
 ** Description: parse rule else if
 ** Param: @struct lexer * @struct parser *parser, struct ast *ast_prev
 ** Return: builded ast
 */
struct ast *parse_rule_elif(struct lexer *lexer, struct parser *parser,
                            struct ast *prev_ast)
{
    struct ast *condition = NULL;
    lexer_pop(lexer);
    condition = parse_compound_list(lexer, parser);
    if (!condition)
        return condition;
    ast_append(parser->nodes, condition);

    struct ast *right = NULL;
    lexer_pop(lexer);
    right = parse_compound_list(lexer, parser);
    if (!right)
        return right;
    ast_append(parser->nodes, right);

    struct ast *wrong = NULL;

    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_ELIF) // TODO
    {
        wrong = parse_rule_else(lexer, parser);
        if (!wrong)
            return wrong;

        ast_append(parser->nodes, wrong);
    }
    else if (token->type == TOKEN_ELSE)
    {
        wrong = parse_rule_else(lexer, parser);
        if (!wrong)
            return wrong;
        ast_append(parser->nodes, wrong);
    }

    struct condition_if_node *node = malloc(sizeof(struct condition_if_node));
    node->condition_c = condition;
    node->then_action = right;
    node->else_action = wrong;

    struct shell_command_node *shell_cmd = prev_ast->node;
    shell_cmd->node = node;
    return prev_ast;
}
