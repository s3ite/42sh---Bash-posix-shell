#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../ast/variable.h"
#include "built_in.h"

#define BUFFER_SIZE 1024
bool is_absolute_path(const char *path)
{
    return path[0] == '/';
}

// check if the directory exists
static bool is_directory_exists(const char *path)
{
    DIR *dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        return true;
    }

    fprintf(stderr, "Directory does not exist\n");
    return false;
}

int my_cd(char **cmd)
{
    char buffer[BUFFER_SIZE];

    struct variable_item *variable = get_variable(variables_list, "OLDPWD");
    char *oldpwd = variable->value.string;
    char *pwd = strdup(getcwd(buffer, BUFFER_SIZE));
    int rc = 0;
    if (cmd[1] != NULL && cmd[2] != NULL)
    {
        fprintf(stderr, "cd : too many arguments");
        return 1;
    }
    // si pas d'arguments
    if (cmd[1] == NULL)
        rc = chdir(getenv("HOME"));

    // cas du -
    else if (strcmp(cmd[1], "-") == 0)
    {
        rc = chdir(oldpwd);
        printf("%s\n", getcwd(buffer, BUFFER_SIZE));
    }
    else if (strcmp(cmd[1], "-/") == 0)
    {
        rc = 2;
        fprintf(stderr, "invalid option");
    }

    // si path absolue / relatif
    else
    {
        if (is_directory_exists(cmd[1]))
            rc = chdir(cmd[1]);
        else
            rc = 1;
    }

    union value value = { .string = getcwd(buffer, BUFFER_SIZE) };
    if (update_variable(variables_list, "PWD", TYPE_STRING, value) == -1)
        add_variable(variables_list, init_item("PWD", value, TYPE_STRING));

    value.string = pwd;
    if (update_variable(variables_list, "OLDPWD", TYPE_STRING, value) == -1)
        add_variable(variables_list, init_item("OLDPWD", value, TYPE_STRING));

    free(pwd);
    return rc;
}
