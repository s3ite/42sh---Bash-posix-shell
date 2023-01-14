#include "ast.h"


struct ast* init_ast(void)
{
    struct ast* ast = malloc(sizeof(struct ast));
    ast->rc.code = 0;
    return ast;
}

/*
 ** Name: ast_append
 ** Description: append ast node to a given ast
 ** struct ast_node ; struct act
 ** Return: void
 */
void ast_append(struct ast_node *list, struct ast *ast)
{
    if (!list->ast && !list->next)
        list->ast = ast;
    else
    {
        while (list)
        {
            if (list->ast == ast)
                return;
            if (!list->next)
                break;
            list = list->next;
        }
        struct ast_node *new_node = malloc(sizeof(struct ast_node));
        new_node->ast = ast;
        new_node->next = NULL;

        list->next = new_node;
    }
}


struct ast_node *ast_list_init()
{
    struct ast_node *res = malloc(sizeof(struct ast_node));

    res->ast = NULL;
    res->next = NULL;

    return res;
}
