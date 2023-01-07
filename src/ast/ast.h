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


union rules
{
    struct condition_if_node if_condition;
    // struct rule_for rule_for;
    // struct rule_while rule_while;
    // struct rule_until rule_until;
    // struct rule_case rule_case;    
};

struct shell_command_node
{
    struct dlist *compound_list; //
    union rule *rules; //{for, while, until, case, if}
};

enum node_type {
    INPUT,
    LIST,
    AND_OR,
    PIPELINE,
    COMMAND ,
    SIMPLE_COMMAND,
    SHELL_COMMAND,
    IF,
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