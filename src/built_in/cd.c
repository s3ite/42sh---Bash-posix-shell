#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "built_in.h"

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
