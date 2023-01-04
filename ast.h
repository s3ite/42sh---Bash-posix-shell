#ifndef AST_H
#define AST_H

#include 'commands.h';
union node_data
{
    // struct node_if node_if;
    // struct node_command node_command;
    struct node_simple_command node_simple_command;
};

enum node_type
{
    // NODE_IF,
    // NODE_COMMAND,
    NODE_SIMPLE_COMMAND,
};

struct ast_node
{
    union node_data data;
    struct node_type type;
};

#endif /* AST_H */