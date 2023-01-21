#define _POSIX_SOURCE

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "built_in.h"


int str_to_int(char *input)
{
    int result = 0;
    if (input[0] == '-')
    {
        input++;
        result = -1;
    }

    for (size_t i = 0; i < strlen(input); i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
        {
            result *= 10;
            result += input[i] - '0';
        }
        else 
            return 2;
    } 


    return result  < 0 ? 255 : result;
}
int my_exit(char **cmd)
{
    if (cmd[1] == NULL) return 0;

    if (cmd[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }

    int res = str_to_int(cmd[1]);
    if (res == 2)
    {
        fprintf(stderr, "exit: invalid argument\n");
    }
    return res;
}
