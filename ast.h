#ifndef AST_H
#define AST_H

struct condition_if_node
{
    struct ast *condition_c; //ast associé à la condition
    struct ast *then_action; //action si autorisée
    struct ast *else_action; //Action si faux 
};


struct simple_command_node
{
    struct str_list *prefix; //commande & args
    struct str_list *values; //valeur après la cmd
    struct red *red; // les redirections de la commande
};

enum node_type {
    CONDITIONS,
    SIMPLE_COMMAND,
};

struct ast{
    enum node_type node_type;
    void *node;
};

struct ast_nodes{
    struct ast *ast;
    struct ast_nodes *next;
};



#endif /* ! AST_H */