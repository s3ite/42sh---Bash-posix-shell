#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
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

static int handle_tiret(char **cmd, char *oldpwd, char buffer[])
{
    int rc = 0;
    if (isspace(cmd[1][1]) != 0)
    {
        rc = 2;
        fprintf(stderr, "invalid option");
    }
    if (strcmp(oldpwd, "") == 0)
    {
        fprintf(stderr, "OLDPWD is not set\n");
        return 1;
    }
    else
    {
        rc = chdir(oldpwd);
        printf("%s\n", getcwd(buffer, BUFFER_SIZE));
    }

    return rc;
}

int my_cd(char **cmd)
{
    if (cmd[1] && cmd[2])
    {
        fprintf(stderr, "cd : too many arguments");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char *oldpwd = get_variable(variables_list, "OLDPWD")->value.string;
    char pwd[BUFFER_SIZE];
    getcwd(pwd, BUFFER_SIZE);

    int rc = 0;

    if (cmd[1] == NULL)
    {
        char *tmp = getenv("HOME");
        if (tmp == NULL)
        {
            fprintf(stderr, " HOME environment variable not set\n");
            return 1;
        }
        rc = chdir(tmp);
    }
    // cas du -
    else if (cmd[1][0] == '-')
    {
        if ((rc = handle_tiret(cmd, oldpwd, buffer)) != 0)
            return rc;
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

    return rc;
}
