#include "print_ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "list.h"

const char *tokenTypeTable(struct token *token)
{
    static const char *tokenTypes[] = {
        [TOKEN_IF] = "TOKEN_IF",
        [TOKEN_THEN] = "TOKEN_THEN",
        [TOKEN_ELIF] = "TOKEN_ELIF",
        [TOKEN_ELSE] = "TOKEN_ELSE",
        [TOKEN_FI] = "TOKEN_FI",
        [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
        //[TOKEN_QUOTE] = "TOKEN_QUOTE",
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

    if (ast->node_type < 8)
        return nodeTypes[ast->node_type];

    return "UNKNOWN";
}

void print_ast(struct parser *parser)
{
    printf("root node %s ", nodeTypeTable(parser->ast));
    struct ast_node *node = NULL;
    node = parser->nodes;
    printf("parser->nodes = %d",parser->nodes->ast->node_type);
    for (; node; node = node->next)
    {
        printf(" %s ", nodeTypeTable(node->ast));
    }
}

void print_ast_bis(struct ast *ast)
{
    if (!ast)
        return;

    if (ast->node_type == OPERATOR)
    {
        struct operator_node *op = ast->node;
        printf("Valeur opérator: ;\n");
        if (op->left)
            print_ast_bis(op->left);
        if (op->right)
            print_ast_bis(op->right);
    }
    if (ast->node_type == SIMPLE_COMMAND)
    {
        struct simple_command_node *node = ast->node;
        printf("Commande:\n");
        dlist_print(node->prefix);
        dlist_print(node->values);
        printf("%s\n", "");
    }
    if (ast->node_type == SHELL_COMMAND)
    {
        printf("-Shell Command \n");

        struct shell_command_node *cmd = ast->node;
        if (cmd->type == IF)
        {
            struct condition_if_node *node = cmd->node;
            if (node->condition_c)
                print_ast_bis(node->condition_c);
            if (node->then_action)
                print_ast_bis(node->then_action);
            if (node->else_action)
                print_ast_bis(node->else_action);
        }
    }
}

/*void print_ast(struct lexer *lexer)
{

    for (size_t i = 0; i < lexer->size; i++) {

        if (lexer->data[i]->type == TOKEN_SEMICOLON)
            printf(" } ");

        else if (lexer->data[i]->type == TOKEN_EOF)
            continue;

        else if (lexer->data[i]->type == TOKEN_NEWLINE || lexer->data[i]->type
== TOKEN_FI) printf("\n");

        else if (lexer->data[i]->type == WORD)
            printf("COMMAND: %s ", lexer->data[i]->value);

        else
            printf("%s { ", tokenTypeTable(lexer->data[i]));
    }
}
*/
