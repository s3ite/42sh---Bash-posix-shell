#define _POSIX_SOURCE
#include "exec/exec.h"
#include "lexer/lexer.h"
#include "parse_command_line/parse_command_line.h"
#include "parser/parser.h"
#include "built_in/built_in.h"

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void exit_program(int signo)
{
  if (signo == SIGTERM)
  {
    printf("End of the instance...");

    // catch des signaux et cleanup des donnnees / trap
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    // on recupere le pid de l'instance actuelle
    pid_t shell_pid = getpid();

    // free of datas allocated
    //TODO:

    // on envoie un SIGQUIT au shell
    kill(shell_pid, SIGQUIT);
  }
}

int main(int argc, char **argv)
{
    if (signal(SIGTERM, exit_program) == SIG_ERR)
      printf("\ncan't catch SIGTERM\n");


    char *input = parse_command_line(argc, argv);
    if (!input)
    {
        while (1)
        {
            char *str = malloc(1024);
            printf("42sh$ ");
            fgets(str, 1024, stdin);
            str[strlen(str) - 1] = '\0';

            struct lexer *lexer = lexer_init(10, str);
            lexer = lexer_load(str, lexer);
            lexer_print(lexer);
            int rc = parse(lexer);
            lexer_destroy(lexer);
            free(input);
            if(rc == RC_ERROR)
            {
                free(str);
                return RC_ERROR;
            }
            free(str);
        }
    }

    struct lexer *lexer = lexer_init(10, input);
    lexer = lexer_load(input, lexer);

    // representation of the ast
    // print_ast(lexer);

    int rc = parse(lexer);

    lexer_destroy(lexer);
    free(input);
    if (rc == RC_ERROR)
        return RC_ERROR;

    return 0;
}
