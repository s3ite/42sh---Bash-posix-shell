#define _POSIX_C_SOURCE 200809L
#include "expansion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "../ast/variable.h"
#include "../run_program.h"
#define BUFFER_SIZE 1024
#define DELIM "$\0"

static char *get_special_var(char *input, char *buffer)
{
    if (strcmp(input, "@") == 0)
        buffer = "";
    else if (strcmp(input, "*") == 0)
        buffer = "";
    else if (strcmp(input, "?") == 0)
    {
        struct variable_item *item = get_variable(variables_list, "?");
        sprintf(buffer, "%d", item->value.integer);
    }
    else if (strcmp(input, "$") == 0)
        sprintf(buffer, "%d", getpid());

    else if (strcmp(input, "#") == 0)
        buffer = "0";
    else if (strcmp(input, "RANDOM") == 0)
        sprintf(buffer, "%d", rand());

    else if (strcmp(input, "PWD") == 0)
        getcwd(buffer, BUFFER_SIZE);

    else if (strcmp(input, "UID") == 0)
        buffer = getenv("UID");

    else if (strcmp(input, "OLDPWD") == 0)
        buffer = getenv("OLDPWD");
    else if (strcmp(input, "IFS") == 0)
        buffer = "";

    return buffer;
}

bool contains_expansions(char *input)
{
    char *tmp = strchr(input, '$');
    if (tmp == NULL)
        return false;

    return isspace(tmp[1]) == 0;
}

/**
 * retrieve the closer {} or ()
 */
static char *get_word_between_closer(char **input, char closer)
{
    char *closer2 = closer == '(' ? ")" : "}";
    char *word = strchr(*input, '$');
    if (word == NULL || word[1] != closer)
        return NULL;

    word = strtok_r(word + 2, closer2, input);
    return word;
}

static char *get_word(char **input)
{
    // characters special $$
    if (input[0][1] == '$')
    {
        *input += 2;
        return "$";
    }

    char *word = strchr(*input, '$');
    if (word == NULL)
        return NULL;

    word = get_word_between_closer(input, '{');
    if (word != NULL)
    {
        if (strchr(word, '$') != NULL)
            return NULL;
        return word;
    }
    word = strtok_r((*input)++, DELIM, input);
    return word;
}

char *expand_variable(char *input, struct variables_list *list)
{
    char *save = input;
    char *new_input = calloc(1, BUFFER_SIZE);
    size_t tmplen = strcspn(input, DELIM);
    char *tmp = strndup(input, tmplen);
    new_input = strncat(new_input, tmp, tmplen);
    input += tmplen;

    if (tmplen < strlen(input))
    {
        char *word = NULL;

        struct variable_item *var;
        word = get_word(&input);
        if (word)
        {
            char *value = NULL;

            if (strchr(word, ' ') != NULL)
            {
                free(new_input);
                fprintf(stderr, "Variable substitution : expand_variable");
                return NULL;
            }

            var = strcmp(word, "?") != 0 ? get_variable(list, word) : NULL;
            if (var != NULL)
            {
                value = get_value(var);
                new_input = strcat(new_input, value);
            }
            else
            {
                char res[BUFFER_SIZE] = { 0 };
                value = get_special_var(word, res);
                new_input = strcat(new_input, value);
            }
        }
        else
        {
            free(new_input);
            fprintf(stderr, "bad substitution : expand_variable");
            free(save);
            free(tmp);
            return NULL;
        }

        tmplen = strcspn(input, "\0");
        free(tmp);
        tmp = strndup(input, tmplen);
        if (tmplen > 0)
            new_input = strcat(new_input, "$");
        new_input = strncat(new_input, tmp, tmplen);
    }

    free(save);
    free(tmp);
    return new_input;
}

static int exec_word(char **new_input, char *word)
{
    int fd[2];
    int status;
    pipe(fd);

    int nbread = 0;
    pid_t pid = fork();
    if (pid == 0)
    { 
        dup2(fd[1], 1); // redirect stdout to the pipe
        close(fd[0]);
        close(fd[1]);
        execl("/bin/sh", "sh", "-c", word, (char *)0);
    }
    else
    {
        if (waitpid(pid, &status, 0) != pid)
            return -1;
        close(fd[1]);
        char *output = calloc(1, BUFFER_SIZE);
        nbread = read(fd[0], output, BUFFER_SIZE);
        close(fd[0]);

        output[nbread - 1] = '\0';
        *new_input = strcat(*new_input, output);
        free(output);
    }

    return 0;
}


/**
 * Build a new string with subtitution commands extended
 */
char *expand_substitution(char *input, struct variables_list *list)
{
    char *new_input = calloc(1, BUFFER_SIZE);
    char *tmp = strtok(input, DELIM);
    char *word = NULL;
    word = strdup(get_word_between_closer(&input, '('));
    while (contains_expansions(word))
    {
        word = expansion_handler(word, list);
        if (word == NULL)
        {
            free(new_input);
            return NULL;
        }
    }

    if (word)
        exec_word(&new_input, word);
    else
        new_input = strcat(new_input, tmp);

    free(word);
    return new_input;
}

char *expansion_handler(char *input, struct variables_list *list)
{
    char *tmp = strchr(input, '$');
    if (tmp == NULL)
        return input;

    if (tmp[1] == '(')
        return expand_substitution(input, list);

    return expand_variable(input, list);
}
