#include "parser.h"

#include <stdlib.h>

#include "../ast/print_ast.h"
#include "../exec/exec.h"

void set_rc(int res, struct global_var *ret) { ret->rc = res; }

int get_rc(struct global_var *ret) { return ret->rc; }

/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
int parse(struct lexer *lexer) {
  // Check for eof or newline

  struct parser *parser = malloc(sizeof(struct parser));
  if (!parser)
    return RC_ERROR;
  parser->ast = NULL;
  parser->nodes = ast_list_init();

  int rc = parse_input(lexer, parser);

  if (!rc) // TODO: Free all structures to avoid memory leak.   // Then// return error code.
  {
    struct token *token = lexer_peek(lexer);
    if(token->type == TOKEN_EOF)
    {
      fprintf(stderr, "Invalid grammar: GET EOF\n");
      parser_free(parser);
      return RC_ERROR;
    }
  }

  rc = ast_exec(parser->ast);

  parser_free(parser);

  return rc;
}

static struct ast *build_operator_node(enum operator_type type,
                                       struct ast *left, struct ast *right) {
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

static int is_condition_token(struct token *token) {
  if (!token)
    return 0;
  return token->type == TOKEN_AND || token->type == TOKEN_OR;
}

struct ast *parse_and_or(struct lexer *lexer, struct parser *parser) {
  struct ast *ast = parse_pipeline(lexer, parser);
  if (!ast)
    return ast;
  ast_append(parser->nodes, ast);
  struct token *token = lexer_peek(lexer);

  struct ast *pipe = NULL;

  while (is_condition_token(token)) {
    if (!token)
      break;

    struct token *copy = token_init(token->value, token->type);

    lexer_pop(lexer);
    token = lexer_peek(lexer);

    if ((pipe = parse_pipeline(lexer, parser)) == NULL) {
      token_free(copy);
      return pipe;
    }
    ast_append(parser->nodes, pipe);

    if (copy->type == TOKEN_OR) {
      ast = build_operator_node(OR, ast, pipe);
      ast_append(parser->nodes, ast);
    }
    if (copy->type == TOKEN_AND) {
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

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser) {
  struct ast *ast = parse_command(lexer, parser);

  ast_append(parser->nodes, ast);
  return ast;
}

void node_free(struct ast_node *nodes) {
  if (!nodes)
    return;
  node_free(nodes->next);
  ast_free(nodes->ast);
  free(nodes);
}

void ast_free(struct ast *ast) {
  if (ast && ast->node_type == SIMPLE_COMMAND) {
    free_ast_simple_command(ast);
  }
  if (ast && ast->node_type == SHELL_COMMAND) {
    struct shell_command_node *node = ast->node;
    free(node->node);
    free(node);
  }
  if (ast && ast->node_type == OPERATOR) {
    struct operator_node *op = ast->node;
    free(op);
  }
  if (ast && ast->node_type == REDIRECTION) {
    struct redirection_node *rd_node = ast->node;
    free_ast_redirection(rd_node);
  }
  if (ast)
    free(ast);
}

/**
 * Free the parser structure
 */
void parser_free(struct parser *parser) {
  // ast_free(parser->ast);
  node_free(parser->nodes);
  free(parser);
}
