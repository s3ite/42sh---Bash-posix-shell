#include "parser.h"

#include <stdlib.h>

#include "../ast/print_ast.h"
#include "../exec/exec.h"

/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
int parse(struct lexer *lexer)
{
    // Check for eof or newline

    struct parser *parser = malloc(sizeof(struct parser));
    if (!parser)
        return RC_ERROR;
    parser->ast = NULL;
    parser->nodes = ast_list_init();

    int rc = parse_input(lexer, parser);

    if (rc == RC_ERROR) // TODO: Free all structures to avoid memory leak. Then
                        // return error code.
        return RC_ERROR;

    ast_exec(parser->ast);

    // representing the ast
  //  print_ast_bis( parser->ast);

    parser_free(parser);

    return RC_SUCCESS;
}

struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{
    struct ast *ast = parse_pipeline(lexer, parser);
    return ast;
}

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{
    return parse_command(lexer, parser);
}



void node_free(struct ast_node *nodes)
{
    if (!nodes)
        return;
    node_free(nodes->next);
    ast_free(nodes->ast);
    free(nodes);

}

void ast_free(struct ast *ast)
{
    if (ast && ast->node_type == SIMPLE_COMMAND)
    {
         free_ast_simple_command(ast);

    }
    if (ast && ast->node_type == SHELL_COMMAND)
    {
        struct shell_command_node *node  = ast->node;
        free(node->node);
        free(ast->node);
    }
    if (ast && ast->node_type == OPERATOR)
    {
        struct operator_node *op = ast->node;
        if (op->right != NULL)
            ast_free(op->right);
        free(ast->node);
    }
    free(ast);
}

void parser_free(struct parser *parser)
{
    //ast_free(parser->ast);
    node_free(parser->nodes);
    free(parser);

}
