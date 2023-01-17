#include "../ast/print_ast.h"
#include "parser.h"

/*
 ** Name: parse_input
 ** Description: parse input
 ** @struct lexer; struct parser
 ** Return: Success code
 */
int parse_input(struct lexer *lexer, struct parser *parser) {
  
  struct token *token = lexer_peek(lexer);
  while (token->type == TOKEN_NEWLINE) {
    lexer_pop(lexer);
    token = lexer_peek(lexer);
  }
  
  if (token->type == TOKEN_EOF)
    return 1;


  token = lexer_peek(lexer);
  if (!token)
    return RC_SUCCESS;

  struct ast *root = parse_list(lexer, parser);
 
  if (!root)
    return 0;

  parser->ast = root;

  return 1;
}
