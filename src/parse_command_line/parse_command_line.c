#define _POSIX_C_SOURCE 2

#include "parse_command_line.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_file_size(FILE *f)
{
    fseek(f, 0L, SEEK_END);
    size_t res = ftell(f);
    fseek(f, 0, SEEK_SET);
    return res;
}

char *parse_command_line(int argc, char **argv)
{
    if (argc == 3)
    {
        int opt;
        size_t l;
        char *a;
        while ((opt = getopt(argc, argv, "c:")) != -1)
        {
            switch (opt)
            {
            case 'c':
                l = strlen(optarg);
                a = malloc(l + 1);
                strcpy(a, optarg);
                return a;
            }
        }
    }
    if (argc == 2)
    {
        FILE *f;
        if ((f = fopen(argv[optind], "r")))
        {
            size_t l = get_file_size(f);
            char *buff = malloc(l + 1);
            if (!buff)
            {
                return NULL;
            }
            fread(buff, l, 1, f);
            buff[l] = '\0';
            fclose(f);
            return buff;
        }
    }
    if (argc == 1)
    {
        if ((isatty(STDIN_FILENO)))
        {
            return NULL;
        }
        int l = 1024;
        size_t red = 0;
        char *buff = malloc(l + 1);
        if (!buff)
        {
            return NULL;
        }
        size_t i = fread(buff, sizeof(char), l, stdin);
        red += i;
        if (red == 0)
        {
            return NULL;
        }
        while (i == 1024)
        {
            l += 1024;
            buff = realloc(buff, l + 1);
            i = fread(buff + red, sizeof(char), 1024, stdin);
            red += i;
        }
        buff[red] = '\0';
        return buff;
    }
    return NULL;
}
/*int main (int argc, char **argv)
{
    char *res=parse_command_line(argc,argv);
    if (res)
    {
        printf("%s\n",res);
        free(res);
    }
}*/
