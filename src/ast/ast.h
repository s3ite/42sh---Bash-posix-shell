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

enum shell_type
{
    IF = 0,
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
    enum shell_type type; //
    void *node;
};


enum operator_type
{
    AND = 0,
    OR, 
    BITAND,
    SEMICOLON
};


struct operator_node
{
    enum operator_type type; 
    struct ast *left; 
    struct ast *right; 
};

enum node_type {
    INPUT,
    LIST,
    AND_OR,
    PIPELINE,
    COMMAND ,
    SIMPLE_COMMAND,
    SHELL_COMMAND,
    OPERATOR,
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
int exec_if(struct shell_command_node *shell);
struct ast_node *ast_list_init();
int ast_exec(struct ast *node);
#endif /* ! AST_H */
