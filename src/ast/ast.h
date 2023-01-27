#ifndef AST_H
#define AST_H
#include <stddef.h>
#include <stdlib.h>

#include "../lexer/lexer.h"

struct RC
{
    int code;
    char error[1024];
};

struct condition_if_node
{
    struct ast *condition_c; // ast associé à la condition
    struct ast *then_action; // action si autorisée
    struct ast *else_action; // Action si faux
};

enum loop
{
    UNTIL = 1,
    WHILE = 2,
};

enum shell_type
{
    IF = 0,
    WU = 1,
    BLOCK = 2
};

struct condition_wu
{
    struct ast *condition;
    struct ast *body;
    enum loop type;
    // 0 = While
    // 1 = Untile
};

enum redirection_type
{
    FD_OUT, // >
    FD_OUT_APPEND, // >>
    FD_OUT_NO_CLOBBER, // >|
    FD_DUP_OUT, // >&

    FD_IN, // <
    FD_DUP_IN, // <&

    FD_IO // <>
};

struct redirection_node
{
    enum redirection_type type;
    int io_number;
    char *word;

    int save_io_number;
    int new_value;

    int save_io_number2;
    int new_value2;
};


struct simple_command_node
{
    struct dlist *args; // commande & args
    struct dlist *values; // valeur après la cmd
    struct ast **prefix; // redirection && assignement
    int prefix_len;
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

struct block_node
{
    enum TokenType type; //{ } or ()
    struct ast *ast;
};

enum operator_type
{
    AND = 0,
    OR,
    BITAND,
    SEMICOLON,
    NEG,
    PIPE
};

struct operator_node
{
    enum operator_type type;
    struct ast *left;
    struct ast *right;
};

enum node_type
{
    INPUT,
    LISTE,
    AND_OR,
    PIPELINE,
    COMMAND,
    SIMPLE_COMMAND,
    SHELL_COMMAND,
    OPERATOR,
    PREFIX,
    REDIRECTION,
    ELEMENT,
    FUNCTION,
};

/**
 * node_type = type du noeud de l'ast
 * node = value du noeud de l'ast
 */
struct ast
{
    enum node_type node_type;
    void *node;
};

struct ast_node
{
    struct ast *ast;
    struct ast_node *next;
};

struct ast *init_ast(void);
void ast_append(struct ast_node *list, struct ast *ast);
int exec_if(struct shell_command_node *shell);
struct ast_node *ast_list_init(void);
int ast_exec(struct ast *node);
#endif /* ! AST_H */
