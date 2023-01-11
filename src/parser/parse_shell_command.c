#include "parser.h"

#include "../ast/list.h"
#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>



struct ast *build_shell_command_node(enum shell_type type)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = SHELL_COMMAND;

    struct shell_command_node *node = malloc(sizeof(struct shell_command_node));
    node->type = type;
    node->node = NULL;

    ast->node = node;

    return ast;
}



struct ast *parse_cmd(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = NULL;
    struct token *token = lexer_peek(lexer);
    if(token->type == TOKEN_IF)
    {
        ast = build_shell_command_node(IF);
         if(!ast)
            printf("%s\n", "pase_cmd ast null");

        ast_append(parser->nodes, ast);
        ast = parse_rule_if(lexer,parser,ast); 
        if(!ast)
            printf("%s\n", "pase_cmd if null");
    }

    return ast;
}



/**
 * Arg1 :  struct provide by the lexer
 * Arg2 : struct provide by the parse function
 * return : Return Code
 * 
 * Goal : Parse a simple command following the grammar, create a simple command node
*/
struct ast *parse_shell_command(struct lexer *lexer, struct parser *parser)
{

    struct ast *ast =NULL;
    ast = parse_cmd(lexer,parser);
    if(!ast)
            printf("%s\n", "parse_command null");

    return ast;
}
