#ifndef COMMANDS
#define COMMANDS
#include <stddef.h>

struct simple_command
{
    char **words;
    size_t num_words;
};

struct condition_if_node
{
    struct ast *condition; //ast associé à la condition
    struct ast *then; //action si autorisée
    struct ast *else; //Action si faux 
};


struct simple_command_node
{
    struct str_list *prefix; //commande & args
    struct str_list *values; //valeur après la cmd
    struct red *red; // les redirections de la commande
};

#endif /* COMMANDS */