#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int my_cd(char **argv) 
{
    char dir[4096] = {0};
    if (argv[2] == NULL)
    {
        strcpy(dir, "/");
        if (dir == NULL)
            return -1;
    } 
    else
    { 
        if (strcmp(argv[2],"-") == 0)
        {
            if (getenv("OLDPWD") == NULL)
            {
                perror("OLDPWD error");
            }
            strcpy(dir, getenv("OLDPWD"));
            if (dir == NULL)
                return -1;
        }
        else
        {
            
            if (getenv("PWD") == NULL) 
            {
                perror("getenv error");
                return 1;
            } 
            strcpy(dir,getenv("PWD"));
            strcat(dir,"/");
            strcat(dir, argv[2]);
        }
    }
    printf("cd : %s\n", dir );
    char acc_dir[100] = {0};
    getcwd(acc_dir,sizeof(acc_dir));
    setenv("OLDPWD",acc_dir,1);
    if (chdir(dir) != 0) 
    {
        return -1;
        printf("RELOU\n");
    }
    char ac_dir[100] = {0};
    getcwd(ac_dir,sizeof(acc_dir));
    setenv("PWD",ac_dir,1);

    return 0;
}