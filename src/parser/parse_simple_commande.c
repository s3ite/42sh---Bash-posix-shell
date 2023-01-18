#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../ast/list.h"
#include "parser.h"

static bool is_prefix(struct token * token)
{
    return token->type == TOKEN_REDIRECTION;
}

static struct ast *handle_prefix(struct lexer *lexer, struct parser *parser)
{
  struct ast *ast = parser_prefix(lexer, parser);

  if (!ast)
    return NULL;
    
  lexer_pop(lexer);
  struct token *token = lexer_peek(lexer);

  if( token->type == WORD )
  {
    while (token->type == WORD) {
      ast->node_type = ELEMENT;
      ast->node = malloc(1024);
      ast->node = strcpy(ast->node, token->value);

      ast_append(parser->nodes, ast);
      token = lexer_peek(lexer);
    }

    return ast;
  }

  while(is_prefix(token))
  {
    ast = parser_prefix(lexer, parser);
    if (ast == NULL)
    {
      fprintf(stderr,"Error parsing handle prefix");
      return NULL;
    }
    ast_append(parser->nodes, ast);
    token = lexer_peek(lexer);
  }
  
  return ast;
}
/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 *
 * Goal : Parse a simple command following the grammar, create a simple command
 * node
 */
struct simple_command_node *parse_simple_commande(struct lexer *lexer,
                                                  struct parser *parser) 
{
    struct token *token = lexer_peek(lexer);
    if (token->type == WORD)
    {
      struct simple_command_node *simple_command = malloc(sizeof(struct simple_command_node));

      struct token *next_token = lexer_pop(lexer);
        // revoir limplemntation de la liste pour placer le premier token
      struct dlist *args = dlist_init();
      struct dlist *values = dlist_init();

      dlist_push_front(args, next_token->value);

      next_token = lexer_peek(lexer);
        
      // Si 1er toker est un args -> ajouter dans args
      if (next_token->value[0] == '-') {
        dlist_push_back(args, next_token->value);
        lexer_pop(lexer);
      } 

      while (next_token->type == WORD) {
        next_token = lexer_peek(lexer);
        if (next_token->type != WORD)
          break;
        
        dlist_push_back(values, next_token->value);
        lexer_pop(lexer);
      }

      simple_command->args = args;
      simple_command->values = values;

      simple_command->prefix = handle_prefix(lexer, parser);

      return simple_command;
    }
    return NULL;
}

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser) {
  struct token *token = lexer_peek(lexer);

  struct ast *ast = init_ast();
  ast->node_type = SIMPLE_COMMAND;

  if (token->type != WORD  && is_prefix(token) == false)
    return NULL;

  struct simple_command_node *sp_cmd = parse_simple_commande(lexer, parser);
  ast->node = sp_cmd;

    
  return ast;
}

void free_simple_command(struct simple_command_node *simple_command) {
  if (simple_command == NULL)
    return;

  free_dlist(simple_command->args);
  free_dlist(simple_command->values);
  free(simple_command);
}

void free_ast_simple_command(struct ast *ast) {
  if (ast == NULL)
    return;

  free_simple_command(ast->node);
}
