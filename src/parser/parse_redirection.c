#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../ast/list.h"
#include "parser.h"

#define DELIM "<>&|"

/**
 * build redirections  node
 */
static struct redirection_node *build_rd_node(int io_number, char *word)
{
    struct redirection_node *rd_node = malloc(sizeof(struct redirection_node));
    rd_node->io_number = io_number;
    rd_node->word = word;
    rd_node->new_value = -1;
    rd_node->new_value2 = -1;
    rd_node->save_io_number = -1;
    rd_node->save_io_number2 = -1;

    return rd_node;
}

/**
 * retrieve the IO_NUMBER present in beginning of the string if preset, -1
 * otherwise, also delete the redirection command io_number from the command
 */
static int get_ionumber(char *value)
{
    int result = -1;

    for (; *value != '\0' && isdigit(*value); value++)
    {
        if (result == -1)
            result = 0;
        result = result * 10 + (*value - '0');
    }

    return result;
}

/**
 * check the redirection type input and create the appropriate ast node
 */
static enum redirection_type input_node(char *command)
{
    if (strcmp(command, "<") == 0)
        return FD_IN;
    else if (strcmp(command, "<&") == 0)
        return FD_DUP_IN;

    return -1;
}

/**
 * check the redirection type input and create the appropriate ast node
 */
static enum redirection_type output_node(char *command)
{
    if (strcmp(command, ">") == 0)
        return FD_OUT;
    else if (strcmp(command, ">>") == 0)
        return FD_OUT_APPEND;
    else if (strcmp(command, ">|") == 0)
        return FD_OUT_NO_CLOBBER;
    else if (strcmp(command, ">&") == 0)
        return FD_DUP_OUT;

    return -1;
}

static enum redirection_type io_node(char *command)
{
    if (strcmp(command, "<>") == 0)
        return FD_IO;

    return -1;
}

// return true if rd_type hasn't the right type
static bool is_unassigned(int rd_type)
{
    return rd_type != FD_OUT && rd_type != FD_DUP_OUT
        && rd_type != FD_OUT_APPEND && rd_type != FD_OUT_NO_CLOBBER
        && rd_type != FD_IN && rd_type != FD_DUP_IN && rd_type != FD_IO;
}

static char *get_word(char **io_command)
{
    char *lastchar = strrchr(*io_command, '|');
    if (lastchar == NULL)
        lastchar = strrchr(*io_command, '&');
    if (lastchar == NULL)
        lastchar = strrchr(*io_command, '>');
    if (lastchar == NULL)
        lastchar = strrchr(*io_command, '<');

    if (lastchar == NULL)
    {
        fprintf(stderr, "erreur de parsing : get word");
        return NULL;
    }

    if (*(++lastchar) == '\0')
        return NULL;

    char *res = strdup(lastchar);
    int len = strlen(res);
    int cmd = strlen(io_command[0]);
    io_command[0][cmd - len] = '\0';
    *lastchar = '\0';
    return res;
}

struct ast *parse_redirection(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);

    if (token->type != TOKEN_REDIRECTION)
    {
        fprintf(stderr, "bad parsing");
        return NULL;
    }

    int io_number = get_ionumber(token->value);
    char *io_command = strchr(token->value, '<');
    char *word = NULL;

    if (io_command == NULL)
        io_command = strchr(token->value, '>');

    word = get_word(&io_command);

    lexer_pop(lexer);
    if (word == NULL)
    {
        token = lexer_peek(lexer);
        if (token->type != TOKEN_EOF && token->type != TOKEN_SEMICOLON)
            lexer_pop(lexer);

        word = strdup(token->value);
    }
    struct redirection_node *rd_node = build_rd_node(io_number, word);

    enum redirection_type rd_type;
    rd_type = input_node(io_command);
    if (is_unassigned(rd_type))
        rd_type = output_node(io_command);
    if (is_unassigned(rd_type))
        rd_type = io_node(io_command);
    if (rd_type < 0)
        fprintf(stderr, "bad parsing rd_type");

    rd_node->type = rd_type;
    struct ast *ast = init_ast();
    ast->node_type = REDIRECTION;
    ast->node = rd_node; // ! ptr sur var

    ast_append(parser->nodes, ast);

    return ast;
}

void free_ast_redirection(struct redirection_node *rd_node)
{
    free(rd_node->word);
    free(rd_node);
}
