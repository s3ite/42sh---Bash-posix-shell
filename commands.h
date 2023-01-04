#ifndef COMMANDS
#define COMMANDS

#include <stddef.h>

struct simple_command
{
    char **words;
    size_t num_words;
};

#endif /* COMMANDS */