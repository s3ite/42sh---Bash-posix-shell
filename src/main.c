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
#include <strings.h>

#include "./ast/variable.h"
#include <stdlib.h>


struct variables_list *variables_list;

void exit_program(int signo)
{
  variables_list = malloc(sizeof(struct variables_list));
  if (signo == SIGTERM)
  {
    //printf("End of the instance...");

    // catch des signaux et cleanup des donnnees / trap
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    // on recupere le pid de l'instance actuelle
    pid_t shell_pid = getpid();

    // free of datas allocated
    free(variables_list);
    
    // on envoie un SIGQUIT au shell
    kill(shell_pid, SIGQUIT);
  }
}

int main(int argc, char **argv)
{


    variables_list = init_variables_list();

    free(variables_list);
    if (signal(SIGTERM, exit_program) == SIG_ERR)
      fprintf(stderr, "\ncan't catch SIGTERM\n");


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
            //lexer_print(lexer);
            int rc = parse(lexer);
            lexer_destroy(lexer);
            free(input);
            if(rc == RC_ERROR)
            {
                free(str);
                free(variables_list);
                return RC_ERROR;
            }
            free(str);
        }
    }
    char *new_input=remove_escaped_newline(input);
    struct lexer *lexer = lexer_init(10, new_input);
    lexer = lexer_load(new_input, lexer);

    if(!lexer)
    {
      free(variables_list);
      free(input);
      free(new_input);
      fprintf(stderr, "%s", "Syntax error: Unterminated quoted string\n");
      //printf("Syntax error: Unterminated quoted string");
      return 2;//erreur lors du lexing
    }

    //lexer_print(lexer);


    int rc = parse(lexer);

    lexer_destroy(lexer);
    free(input);
    free(new_input);
    if (rc == RC_ERROR)
        return RC_ERROR;

    free_variables(variables_list);
    return rc;
}
