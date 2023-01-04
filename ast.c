#include 'ast.h'

/**
 * Return : ast_node de type simple command
 * arg 1 : vector d
 */
// void simple_commmand_parsing

simple_command *parse(char **words, size_t num_words)
{
    // Allocate memory for the simple_command AST node.
    simple_command *cmd = malloc(sizeof(simple_command));
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for simple_command\n");
        exit(1);
    }

    // Set the word tokens in the simple_command AST node.
    cmd->words = words;
    cmd->num_words = num_words;

    return cmd;
}