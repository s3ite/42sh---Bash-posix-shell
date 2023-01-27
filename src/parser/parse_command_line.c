#define _POSIX_C_SOURCE 2

#include "parse_command_line.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 ** Name: get_file_size
 ** Description: compute file size
 ** File *f
 ** Return: int
 */
int get_file_size(FILE *f)
{
    fseek(f, 0L, SEEK_END);
    size_t res = ftell(f);
    fseek(f, 0, SEEK_SET);
    return res;
}

/*
 ** Name: remove_escaped_newline
 ** Description: remove escaped newline
 ** char *input
 ** Return: char *
 */
char *remove_escaped_newline(char *input)
{
    size_t l = strlen(input);
    char *ret = malloc(l + 1);
    size_t i = 0;
    size_t k = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '\\' && input[i + 1] == '\n')
        {
            i += 2;
        }
        else
        {
            ret[k] = input[i];
            i++;
            k++;
        }
    }
    ret[k] = '\0';
    return ret;
}

/*
 ** Name: get_stdin
 ** Description: read stdin
 ** void
 ** Return: schar *
 */
char *get_stdin()
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

/*
 ** Name: parse_command_line
 ** Description: parse programm argument
 ** int argc, char **argv
 ** Return: char *
 */
char *parse_command_line(int argc, char **argv)
{
    if (argc >= 3)
    {
        int opt;
        size_t l;
        char *a;
        while ((opt = getopt(argc, argv, "c:")) != -1)
        {
            if (opt == 'c')
            {
                l = strlen(optarg);
                a = malloc(l + 1);
                strcpy(a, optarg);
                return a;
            }
        }
    }
    if (argc == 2)
    {
        FILE *file;
        if ((file = fopen(argv[optind], "r")))
        {
            size_t file_len = get_file_size(file);
            char *buffer = malloc(file_len + 1);
            if (!buffer)
            {
                return NULL;
            }
            fread(buffer, file_len, 1, file);
            buffer[file_len] = '\0';
            fclose(file);
            return buffer;
        }
    }
    if (argc == 1)
    {
        return get_stdin();
    }
    return NULL;
}
