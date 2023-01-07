#define _POSIX_C_SOURCE 2

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parse_command_line.h"

size_t get_file_size(FILE *f)
{

    fseek(f, 0L, SEEK_END);
    size_t res = ftell(f);
    fseek(f, 0, SEEK_SET);
    return res;
}

char *parse_command_line(int argc, char **argv)
{
    if(argc == 3)
    {
        int opt;
        size_t l;
        char *a;
        while((opt=getopt(argc, argv, "e:")) != -1)
        {
            switch (opt)
            {
                case 'e':
                    l=strlen(optarg);
                    a=malloc(l+1);
                    strcpy(a,optarg);
                    return a;
            }
        }
    }
    if(argc == 2)
    {
        FILE *f;
        if((f = fopen(argv[optind],"r")))
        {
            size_t l = get_file_size(f);
            char* buff=malloc(l+1);
            if(!buff)
            {
                return NULL;
            }
            fread(buff,l,1,f);
            buff[l]='\0';
            fclose(f);
            return buff;
        }
    }
    if(argc == 1)
    {
        size_t l = get_file_size(stdin);
        char* buff=malloc(l+1);
        if(!buff)
        {
            return NULL;
        }
        fread(buff,l,1,stdin);
        buff[l]='\0';
        return buff;
    }
    return NULL;
}

/*int main (int argc, char **argv)
{
    char *res=parse_command_line(argc,argv);
    printf("%s\n",res);
    free(res);
}*/