#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/ast.h"
#include "built_in.h"

int my_echo(char **cmd, size_t arg_number, size_t index)
{
    int flag_e = 0;
    int flag_n = 0;
    size_t i = index + 1;
    while (i < arg_number)
    {
        if (!strcmp("-n", cmd[i]))
        {
            flag_n = 1;
        }
        else if (!strcmp("-e", cmd[i]))
        {
            flag_e = 1;
        }
        else if (!strcmp("-ne", cmd[i]) || !strcmp("-en", cmd[i]))
        {
            flag_e = 1;
            flag_n = 1;
        }
        else if (!strcmp("-E", cmd[i]))
            flag_e = 0;

        else if (!strcmp("-nE", cmd[i]) || !strcmp("-En", cmd[i]))
        {
            flag_n = 1;
            flag_e = 0;
        }

        else
        {
            break;
        }
        i++;
    }

    size_t start = i;
    while (i < arg_number)
    {
        if (i != start)
        {
            putchar(' ');
        }
        if (!flag_e)
        {
            printf("%s", cmd[i]);
        }
        else
        {
            for (size_t j = 0; cmd[i][j]; j++)
            {
                if (cmd[i][j] == '\\' && cmd[i][j + 1])
                {
                    if (cmd[i][j + 1] == 'n')
                    {
                        putchar('\n');
                        j++;
                    }
                    else if (cmd[i][j + 1] == 't')
                    {
                        putchar('\t');
                        j++;
                    }
                    else if (cmd[i][j + 1] == '\\')
                    {
                        printf("\\");
                        j++;
                    }
                    else
                    {
                        putchar('\\');
                    }
                }
                else
                {
                    putchar(cmd[i][j]);
                }
            }
        }
        i++;
    }

    if (!flag_n)
    {
        putchar('\n');
    }
    return 0;
}
