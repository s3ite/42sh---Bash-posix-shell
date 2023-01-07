#include <stdlib.h>

#include "parser.h"
#include "../exec/exec.h"


struct ast_node *ast_list_init(void)
{
    struct ast_node *res = malloc(sizeof(struct ast_node));

    res->ast = NULL;
    res->next = NULL;

    return res;
}



char **to_command(struct dlist *prefix,struct dlist *values)
{
     if (!prefix || !values)
        return NULL;

    size_t size = dlist_size(prefix) + dlist_size(values);

    struct dlist_item *tmp1 = prefix->head;
    struct dlist_item *tmp2 = values->head;

    char **cmd = malloc(sizeof(char *) * (size + 1));
    size_t i = 0;

    for ( i=0; i < size && tmp1; ++i)
    {
        cmd[i] = strdup(tmp1->value);
        tmp1 = tmp1->next;
    }

    for ( i; i < size && tmp2; ++i)
    {
        cmd[i] = strdup(tmp2->value);
        tmp2 = tmp2->next;
    }

    cmd[size] = NULL;
    return cmd;

}

int simple_cmd_exec(struct ast *ast)
{

    struct simple_command_node *cmd_nbode = ast->node;
   
    struct dlist *prefix = cmd_nbode->prefix;
    struct dlist *values = cmd_nbode->values;

    struct dlist_item *head = prefix->head; 
    char **cmd = to_command(prefix, values);

    int rc = run_command(cmd);



    return rc;
}

int global_exec(struct ast *node)
{
    int success = 0;

    if(node->node_type == SIMPLE_COMMAND)
    {
        printf("%s\n", "HIT !!!!!==>");
        simple_cmd_exec(node);
    }
    else 
    {
        printf("%s\n", "NOT MATCH !!!!!==>");
    }
    return 0;
}


/*
 ** Name: parse
 ** Description: parse given string in lexer input / Entry function
 ** @struct lexer *
 ** Return: Success code
 */
int parse(struct lexer *lexer)
{
    //Check for eof or newline
    
    struct parser *parser = malloc(sizeof(struct parser));
    if(!parser)
        return RC_ERROR;
    parser->ast = NULL;
    parser->nodes = ast_list_init();

    int rc = parse_input(lexer, parser);

    if(rc == RC_ERROR) //TODO: Free all structures to avoid memory leak. Then return error code.
        return RC_ERROR;

        global_exec(parser->ast);

    return RC_SUCCESS;

}



struct ast *parse_and_or(struct lexer *lexer, struct parser *parser)
{
   return parse_pipeline(lexer, parser);
}

struct ast *parse_pipeline(struct lexer *lexer, struct parser *parser)
{
    return parse_command(lexer,parser);
}









