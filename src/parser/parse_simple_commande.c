#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/list.h"
#include "parser.h"

static bool is_prefix(struct token *token)
{
    return token->type == TOKEN_REDIRECTION;
}

static struct ast **handle_prefix(struct lexer *lexer, struct parser *parser, int *prefix_len)
{
    struct ast **ast = NULL;
    struct token *token = NULL;
    int i = 0;
    do
    {
        ast = realloc(ast, sizeof(struct ast *) * (i + 1));
        ast[i] = parser_prefix(lexer, parser);
        if (ast[i] == NULL)
        {
            free(ast);
            return NULL;
        }

        ast_append(parser->nodes, ast[i]);
        token = lexer_peek(lexer);
        i++;
    } while (token != NULL && is_prefix(token));

    *prefix_len = i;
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
    struct simple_command_node *simple_command =
            malloc(sizeof(struct simple_command_node));
    struct token *token = lexer_peek(lexer);
    if (token->type == WORD)
    {


        struct token *next_token = lexer_pop(lexer);
        // revoir limplemntation de la liste pour placer le premier token
        struct dlist *args = dlist_init();
        struct dlist *values = dlist_init();

        dlist_push_front(args, next_token->value);

        next_token = lexer_peek(lexer);

        // Si 1er toker est un args -> ajouter dans args
        if (next_token->value[0] == '-')
        {
            dlist_push_back(args, next_token->value);
            lexer_pop(lexer);
        }

        while (next_token->type == WORD)
        {
            next_token = lexer_peek(lexer);
            if (next_token->type != WORD)
                break;

            dlist_push_back(values, next_token->value);
            lexer_pop(lexer);
        }

        simple_command->args = args;
        simple_command->values = values;

        simple_command->prefix = handle_prefix(lexer, parser, &simple_command->prefix_len);

        //return simple_command;
    }
    else if (token->type == TOKEN_REDIRECTION)
    {
        simple_command->prefix = handle_prefix(lexer, parser, &simple_command->prefix_len);
    }
    else
    {
        free_simple_command(simple_command);
        return NULL;
    }

    return simple_command;
}

struct ast *add_simple_commande(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);

    struct ast *ast = init_ast();
    ast->node_type = SIMPLE_COMMAND;

    if (token->type != WORD && is_prefix(token) == false)
        return NULL;

    struct simple_command_node *sp_cmd = parse_simple_commande(lexer, parser);
    ast->node = sp_cmd;

    return ast;
}
void free_prefix(struct ast **ast)
{
    for (size_t i = 0; i < 2; i++)
    {
        free_ast_redirection(ast[i]->node);
        free(ast[i]);
    }
    free(ast);
}

void free_simple_command(struct simple_command_node *simple_command)
{
    if (simple_command == NULL)
        return;

    free_dlist(simple_command->args);
    free_dlist(simple_command->values);
    free(simple_command->prefix);
    free(simple_command);
}

void free_ast_simple_command(struct ast *ast)
{
    if (ast == NULL)
        return;

    free_simple_command(ast->node);
}
