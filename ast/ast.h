#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include <stddef.h>

struct condition_if_node
{
    struct ast *condition_c; //ast associé à la condition
    struct ast *then_action; //action si autorisée
    struct ast *else_action; //Action si faux 
};


struct simple_command_node
{
    struct dlist *prefix; //commande & args
    struct dlist *values; //valeur après la cmd
    //struct red *red;  les redirections de la commande
};

enum node_type {
    INPUT = "INPUT",
    LIST = "LIST",
    AND_OR = "AND_OR",
    PIPELINE = "PIPELINE",
    COMMAND = "COMMAND",
    SIMPLE_COMMAND = "SIMPLE_COMMAND",
    SHELL_COMMAND = "SHELL_COMMAND",
    IF = "IF",
};

/**
 * node_type = type du noeud de l'ast
 * node = value du noeud de l'ast
*/
struct ast{
    enum node_type node_type;
    void *node;
};

struct ast_node{
    struct ast *ast;
    struct ast_node *next;
};


void ast_append(struct ast_node *list, struct ast *ast);



#endif /* ! AST_H */
