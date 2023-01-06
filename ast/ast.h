#ifndef AST_H
#define AST_H

struct str_list
{
    struct str_list_item *head; 
    struct str_list_item *tail; 
};

struct str_list_item 
{
    char *value; 
    struct str_list_item *next; 
    struct str_list_item *prev; 
    int quote;
};

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
    //struct red *red;  les redirections de la commande
};

enum node_type {
    INPUT,
    LIST,
    AND_OR,
    PIPELINE,
    COMMAND,
    SIMPLE_COMMAND,
    SHELL_COMMAND,
    IF,
    
};

struct ast{
    enum node_type node_type;
    void *node;
};

struct ast_node{
    struct ast *ast;
    struct ast_node *next;
};



#endif /* ! AST_H */