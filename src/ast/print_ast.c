#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "list.h"
#include "print_ast.h"



const char *tokenTypeTable(struct token *token)
{
  static const char *tokenTypes[] = {
    [TOKEN_IF] = "TOKEN_IF",
    [TOKEN_THEN] = "TOKEN_THEN",
    [TOKEN_ELIF] = "TOKEN_ELIF",
    [TOKEN_ELSE] = "TOKEN_ELSE",
    [TOKEN_FI] = "TOKEN_FI",
    [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
    [TOKEN_QUOTE] = "TOKEN_QUOTE",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [WORD] = "WORD",
    [TOKEN_EOF] = "TOKEN_EOF",
  };

  return tokenTypes[token->type];
}

const char *nodeTypeTable(struct ast *ast)
{
  static const char *nodeTypes[] = {
    [INPUT] = "INPUT",
    [LISTE] = "LIST",
    [AND_OR] = "AND_OR",
    [PIPELINE] = "PIPELINE",
    [COMMAND] = "COMMAND",
    [SIMPLE_COMMAND] = "SIMPLE_COMMAND",
    [SHELL_COMMAND] = "SHELL_COMMAND",
    [OPERATOR] = "OPERATOR",
  };

  if (ast->node_type <8 )
    return nodeTypes[ast->node_type];

  return "UNKNOWN";
}

void print_ast(struct parser *parser) {

    printf(" %s ", nodeTypeTable(parser->ast));
    struct ast_node *node = NULL;
    node = parser->nodes;
    
    for (; node ; node = node->next) {
        printf(" %s ", nodeTypeTable(node->ast));
    }
}


/*
const char *tokenTypeTable(struct token *token)
{
  static const char *tokenTypes[] = {
    [TOKEN_IF] = "TOKEN_IF",
    [TOKEN_THEN] = "TOKEN_THEN",
    [TOKEN_ELIF] = "TOKEN_ELIF",
    [TOKEN_ELSE] = "TOKEN_ELSE",
    [TOKEN_FI] = "TOKEN_FI",
    [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
    [TOKEN_QUOTE] = "TOKEN_QUOTE",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [WORD] = "WORD",
    [TOKEN_EOF] = "TOKEN_EOF",
  };

  return tokenTypes[token->type];
}


void print_ast(struct lexer *lexer) {

    for (size_t i = 0; i < lexer->size; i++) {

        if (lexer->data[i]->type == TOKEN_SEMICOLON)
            printf(" } ");

        else if (lexer->data[i]->type == TOKEN_EOF)
            continue;
            
        else if (lexer->data[i]->type == TOKEN_NEWLINE || lexer->data[i]->type == TOKEN_FI)
                printf("\n");

        else if (lexer->data[i]->type == WORD)
            printf("COMMAND: %s ", lexer->data[i]->value);
            
        else
            printf("%s { ", tokenTypeTable(lexer->data[i]));
    }
}


*/