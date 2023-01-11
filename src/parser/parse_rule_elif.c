#include "parser.h"




struct ast *parse_rule_elif(struct lexer *lexer, struct parser *parser, struct ast *prev_ast)
{
   struct ast *condition = parse_condition(lexer, parser, TOKEN_ELIF);
    if (!condition)
        return NULL;

    struct ast *right = parse_condition(lexer, parser, TOKEN_THEN);
    if (!right)
        return NULL;

    struct ast *wrong = NULL;

    struct token *token = lexer_peek(lexer);
    if (token->type == TOKEN_ELIF || token->type == TOKEN_ELSE)
    {
        wrong = parse_rule_else(lexer, parser);
        if (!wrong)
            return NULL;

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
