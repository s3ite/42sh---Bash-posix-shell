#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../ast/ast.h"

#include "../ast/list.h"
#include "parser.h"


#define DELIM "<>&|"

/**
 * build redirections  node 
*/
struct redirection_node *build_rd_node(int io_number, struct ast *command1, struct ast *command2)
{
    struct redirection_node *rd_node = malloc(sizeof(struct redirection_node));
    rd_node->command1 = command1;
    rd_node->command2 = command2;
    rd_node->io_number = io_number; 

    return rd_node;
}


/**
 * check rd_operator simple type
 * return true if input is rd_char node type node false otherwise
*/
bool check_simple_input(char *input, char rd_char)
{
    char *index = strchr(input, rd_char);
    if (index == NULL) {
        return false;
    }

    char prev = *(index - 1);
    char next = *(index + 1);
    if (prev != '<' && prev != '>' && prev != '&' && prev != '|' && next != '<' && next != '>' && next != '&' && next != '|') 
        return true;
        
    return false;
}

/**
 * check rd_operator complex type
 * return true if input is rd_string node type node false otherwise
*/

bool check_complex_input(char *input, char * rd_string)
{
    char *firstChar = strchr(input, rd_string[0]);
    char *lastChar = strrchr(input, rd_string[1]);

    if (firstChar == NULL || lastChar == NULL)
        return false;

    char prev = *(firstChar - 1);
    char next = *(lastChar + 1);
    
    if (prev != '<' && prev != '>' && prev != '&' && prev != '|' && next != '<' && next != '>' && next != '&' && next != '|') 
        return true;
        
    return false;
}
/**
 * retrieve the IO_NUMBER present in beginning of the string if preset, -1 otherwise
*/
int get_ionumber(struct token *token)
{
    int result = -1;
    char *tmp = token->value;
    
    for (; *tmp != '\0' && isdigit(*tmp); tmp++)
    {
        if (result == -1) result = 0;
        result = result * 10 + (*tmp - '0');
    }

    return result;
}

/**
 *  return the string containing last argument
*/
struct ast *get_command_2(struct lexer *lexer, struct parser *parser)
{
    return parse_list(lexer, parser);
}



/**
 * check the redirection type input and create the appropriate ast node
*/
struct ast *build_rd_input_node(struct token *token, struct redirection_node *rd_node)
{
    if (check_simple_input(token->value, '<'))
        rd_node->type = FD_IN;
    else if (check_complex_input(token->value, "<&"))
        rd_node->type = FD_DUP_IN;
    else 
        return NULL;

    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = REDIRECTION;
    ast->node = rd_node;

    return ast;
}


/**
 * check the redirection type input and create the appropriate ast node
*/
struct ast *build_rd_output_node(struct token *token, struct redirection_node *rd_node)
{
    if (check_simple_input(token->value, '>'))
        rd_node->type = FD_OUT;
    else if (check_complex_input(token->value, ">>"))
        rd_node->type = FD_OUT_APPEND;
    else if (check_complex_input(token->value, ">|"))
        rd_node->type = FD_OUT_NO_CLOBBER;
    else if (check_complex_input(token->value, ">&"))
        rd_node->type = FD_DUP_OUT;
    else 
        return NULL;

    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = REDIRECTION;
    ast->node = rd_node;

    return ast;
}


/**
 * check the redirection type input and create the appropriate ast node
*/
struct ast *build_rd_io_node(struct token *token,  struct redirection_node *rd_node)
{
    if (check_complex_input(token->value, "<>"))
        rd_node->type = FD_IO;
    else 
        return NULL;
    
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = REDIRECTION;
    ast->node = rd_node;

    return ast;
}


struct ast *parse_redirection(struct lexer *lexer, struct parser *parser, struct ast *cmd1)
{
    struct ast *ast = NULL;
    struct token *token = lexer_pop(lexer);
    
    if (token->type == TOKEN_REDIRECTION)
    {
        int io_number = get_ionumber(token);

        struct ast *cmd2 = get_command_2(lexer, parser);
        struct redirection_node *rd_node = build_rd_node(io_number, cmd1,  cmd2);

        ast = build_rd_input_node(token, rd_node);
        if (ast == NULL)
            ast = build_rd_output_node(token, rd_node);
        if (ast == NULL)
            ast = build_rd_io_node(token, rd_node);
        if (ast == NULL)
        {
            free(ast);
            free(rd_node);
            return NULL;
        }
        
        ast_append(parser->nodes, ast);
    }
    return ast;
}



void free_ast_redirection(struct redirection_node *rd_node)
{
    (void) rd_node;
    //free_ast_simple_command(rd_node->command1);
    //free_ast_simple_command(rd_node->command2);

    //free(rd_node);
}