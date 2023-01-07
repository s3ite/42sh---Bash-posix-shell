#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "list.h"
#include "print_ast.h"

void print_ast(struct ast_node *head) {
    
struct ast_node *ast_node = head;

    if (!head)
    {
        printf("empty ast_node\n");
        return;
    }

    for(; ast_node; ast_node = ast_node->next)
    {
        
        if(ast_node->ast->node_type !=  SIMPLE_COMMAND) {
            printf("%d (", ast_node->ast->node_type);
            continue;
        }
        else{
            char *command = print_simplecommand(ast_node->ast->node);
            char *arguments = print_simplecommand_args(ast_node->ast->node);

            printf("%s { command %s %s } ", (char *) ast_node->ast->node, command, arguments);
        }
    }
}


char *print_simplecommand(struct simple_command_node *simple_command)
{
    return simple_command->prefix->head->value;
}

char *print_simplecommand_args(struct simple_command_node *simple_command)
{
    return simple_command->values->head->value;
}
