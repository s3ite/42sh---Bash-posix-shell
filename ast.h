#ifndef AST_H
#define AST_H

union node_data
{
    struct node_if node_if;
    struct node_command node_command;
    struct simple_command simple_command;
};

enum node_type
{
    NODE_IF,
    NODE_COMMAND,
    NODE_SIMPLE_COMMAND,
} type;

struct ast_node
{
    union node_data data;
    struct node_type type;
};


#endif /* AST_H */