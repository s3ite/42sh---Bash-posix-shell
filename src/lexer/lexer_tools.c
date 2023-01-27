#include "lexer.h"

int in(char c, char *delim)
{
    size_t i = 0;
    while (delim[i] != '\0' && delim[i] != c)
    {
        i++;
    }
    return (delim[i] == c || c == '\0');
}

char *strnappend(char *str1, char *str2, int n)
{
    if (n == 0 || str2 == NULL || str1 == NULL)
    {
        return str1;
    }
    int i = strlen(str1);
    str1 = realloc(str1, 1 + i + n);
    int j = 0;
    while (j < n)
    {
        str1[i + j] = str2[j];
        j++;
    }
    str1[i + j] = '\0';
    return str1;
}

void strappendchar(char **str1, char c)
{
    char *str = *str1;
    size_t len = strlen(str);
    str = realloc(str, len + 1 + 1);
    str[len] = c;
    str[len + 1] = '\0';
    *str1 = str;
}

char *remove_backslash(char *str)
{
    int i = strlen(str);
    char *ret = malloc(i + 1);
    int j = 0;
    int k = 0;
    while (str[j] != '\0')
    {
        if (str[j] == '\\')
        {
            j++;
            if (str[j] == '\\')
            {
                ret[k] = str[j];
                k++;
                j++;
            }
        }
        else
        {
            ret[k] = str[j];
            k++;
            j++;
        }
    }
    ret[k] = '\0';
    free(str);
    return ret;
}
