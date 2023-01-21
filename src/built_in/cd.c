#define _POSIX_C_SOURCE 200112L
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../ast/variable.h"
#include "built_in.h"

bool is_absolute_path(const char *path)
{
    return path[0] == '/';
}

static int set_pwd(char *path, struct variable_item *pwd)
{
    int rc = setenv("PWD", path, 1);

    if (rc == -1)
        return -1;

    if (update_variable(variables_list, "OLDPWD", TYPE_STRING, pwd->value) != 0)
    {
        fprintf(stderr, "ERROR: variable OLDPWD not set properly");
        return -1;
    }

    if (update_variable(variables_list, "PWD", TYPE_STRING,
                        (union value){ .string = path })
        != 0)
    {
        fprintf(stderr, "ERROR: variable PWD not set properly");
        return -1;
    }
    return 0;
}

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
    if (cmd[2] != NULL)
    {
        fprintf(stderr, "cd : too many arguments");
        return 1;
    }
    struct variable_item *pwd_var = get_variable(variables_list, "PWD");
    struct variable_item *old_pwd_var = get_variable(variables_list, "OLDPWD");

    // si pas d'arguments
    if (cmd[1] == NULL)
        return set_pwd(getenv("HOME"), pwd_var);

    // si path absolue
    if (is_absolute_path(cmd[1]))
        return set_pwd(cmd[1], pwd_var);

    // cas du -
    if (strcmp(cmd[1], "-") == 0)
        set_pwd(old_pwd_var->value.string, pwd_var);

    char *tmp_path = malloc(1024);
    sprintf(tmp_path, "%s/%s", pwd_var->value.string, cmd[1]);

    if (is_directory_exists(tmp_path) == false)
        return 1; // ERROR INVALID PATH

    int rc = set_pwd(tmp_path, pwd_var);
    free(tmp_path);
    return rc;
}

/*
int my_cd(char **argv, int size)
{
    char dir[4096] = { 0 };
    if (size == 1)
    {
        strcpy(dir, "./");
    }
    else
    {
        if (strcmp(argv[1], "-") == 0)
        {
            if (getenv("OLDPWD") == NULL)
            {
                perror("OLDPWD error");
            }
            strcpy(dir, getenv("OLDPWD"));
        }
        else
        {
            if (getenv("PWD") == NULL)
            {
                perror("getenv error");
                return 1;
            }
            strcpy(dir, getenv("PWD"));
            strcat(dir, "/");
            strcat(dir, argv[1]);
        }
    }
    // printf("cd : %s\n", dir );
    char acc_dir[100] = { 0 };
    getcwd(acc_dir, sizeof(acc_dir));
    setenv("OLDPWD", acc_dir, 1);
    if (chdir(dir) != 0)
    {
        return -1;
    }
    char ac_dir[100] = { 0 };
    getcwd(ac_dir, sizeof(acc_dir));
    setenv("PWD", ac_dir, 1);

    return 0;
}
*/