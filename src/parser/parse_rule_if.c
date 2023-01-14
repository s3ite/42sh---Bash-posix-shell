#include "parser.h"

struct condition_if_node *build_condition_if_node(struct ast *condition,
                                                  struct ast *then_action,
                                                  struct ast *else_action) {
  struct condition_if_node *ast = malloc(sizeof(struct condition_if_node));

  ast->condition_c = condition;
  ast->then_action = then_action;
  ast->else_action = else_action;

  return ast;
}

/*
 ** Name: parse_rule_if
 ** Description: parse rule if
 ** Param: @struct lexer * @struct parser *parser, struct ast *ast_prev
 ** Return: builded ast
 */
struct ast *parse_rule_if(struct lexer *lexer, struct parser *parser,
                          struct ast *prev_ast) {
  if (!prev_ast)
    return NULL;
  struct ast *cond = NULL;
  lexer_pop(lexer);
  cond = parse_compound_list(lexer, parser);
  if (!cond)
    return cond;
  ast_append(parser->nodes, cond);

  struct ast *right = NULL;
  lexer_pop(lexer);
  right = parse_compound_list(lexer, parser);

  if (!right)
    return right;
  ast_append(parser->nodes, right);

  struct ast *else_clause = NULL;
  struct token *token = lexer_peek(lexer);
  if (token->type == TOKEN_ELSE) // TODO
  {
    else_clause = parse_rule_else(lexer, parser);
    if (!else_clause)
      return else_clause;
    ast_append(parser->nodes, else_clause);
  } else if (token->type == TOKEN_ELIF) {
    else_clause = parse_rule_else(lexer, parser);
    if (!else_clause)
      return else_clause;
    ast_append(parser->nodes, else_clause);
  }

  token = lexer_peek(lexer);
  if (token->type != TOKEN_FI)
    return NULL;
  lexer_pop(lexer);

  struct condition_if_node *if_node =
      build_condition_if_node(cond, right, else_clause);
  struct shell_command_node *shell_node = prev_ast->node;
  shell_node->node = if_node;

  return prev_ast;
}
