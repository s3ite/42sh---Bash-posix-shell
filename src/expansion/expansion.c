#include "expansion.h"

#include <stdlib.h>
#include <string.h>

char *get_word_between_parenthesis(char *input)
{
    char *word = strchr(input, '$');
    if (word == NULL || word[1] != '(')
        return NULL;

    word = strtok_r(word + 2, ")", &input);
    return word;
}

char *get_word(char *input)
{
    char *word = strchr(input, '$');
    if (word == NULL)
        return NULL;

    word = get_word_between_parenthesis(input);
    if (word != NULL)
        return word;

    word = strtok_r(input, " \n\t\r", &input);
    return word + 1;
}

char *expand_variable(char *input, struct variables_list *list)
{
    char *new_input = calloc(1, 1024);
    char *tmp = strtok(input, " \0\n\t\r");

    char *word = NULL;

    struct variable_item *var;
    for (; tmp; tmp = strtok(NULL, " \n\t\r"))
    {
        word = get_word(tmp);
        if (word)
        {
            var = get_variable(list, word);
            if (var != NULL)
            {
                char *value = get_value(var);
                new_input = strcat(new_input, value);
            }
        }
        else
            new_input = strcat(new_input, tmp);
    }

    free(input);
    return new_input;
}

bool contains_variable(char *input)
{
    char *tmp = strchr(input, '$');
    if (tmp == NULL)
        return false;

    return isspace(tmp[1]) == 0;
}