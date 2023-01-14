#include <stdio.h>
#include <stdlib.h>

#include "../ast/list.h"
#include "parser.h"

/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 *
 * Goal : Parse a simple command following the grammar, create a simple command
 * node
 */

struct simple_command_node *parse_simple_commande(struct lexer *lexer,
                                                  struct parser *parser) {
  (void)parser;
  struct token *next_token = lexer_pop(lexer);

  if (next_token->type != WORD)
    return NULL;

  // revoir limplemntation de la liste pour placer le premier token
  struct dlist *prefix = dlist_init();
  struct dlist *values = dlist_init();

  dlist_push_front(prefix, next_token->value);

  next_token = lexer_peek(lexer);

  // Si 1er toker est un args -> ajouter dans prefix
  if (next_token->value[0] == '-') {
    dlist_push_back(prefix, next_token->value);
  } else {
    dlist_push_back(values, next_token->value);
  }

  while (next_token->type == WORD) {
    lexer_pop(lexer);
    next_token = lexer_peek(lexer);
    if (next_token->type != WORD)
      break;
    else
      dlist_push_back(values, next_token->value);
  }

  struct simple_command_node *simple_command =
      malloc(sizeof(struct simple_command_node));
  simple_command->prefix = prefix;
  simple_command->values = values;

  return simple_command;
}

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser) {
  struct ast *ast = init_ast();
  ast->node_type = SIMPLE_COMMAND;
  ast->node = parse_simple_commande(lexer, parser);

  return ast;
}

void free_simple_command(struct simple_command_node *simple_command) {
  if (simple_command == NULL)
    return;

  free_dlist(simple_command->prefix);
  free_dlist(simple_command->values);
  free(simple_command);
}

void free_ast_simple_command(struct ast *ast) {
  if (ast == NULL)
    return;

  free_simple_command(ast->node);
}
