#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

void print_ast(struct ast_node *head) {
struct ast_node *ast_node = head->ast;
    for(; ast_node; ast_node = ast_node->next)
    {
        if(ast_node->ast->node_type = SIMPLE_COMMAND) {
            print("%s { command %s %s } ", (char *) ast_node->ast->node, print_simplecommand((struct simple_commande *) ast_node->ast), print_simplecommand_args((struct simple_commande *) ast_node->ast));
        }
        else{
            print("autre_noaud");
        }
    }
}
