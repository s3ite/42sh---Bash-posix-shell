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
    if (signal(SIGTERM, exit_program) == SIG_ERR)
      fprintf(stderr, "\ncan't catch SIGTERM\n");


    char *input = parse_command_line(argc, argv);
    if(!input)
    {
      fprintf(stderr, "%s", "The command line syntax is: 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
      return 2;//erreur lors de la lecture de l'input
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
      return 2;//erreur lors du lexing
    }


    int rc = 0;
    struct parser *parser = parse(lexer);
    if(parser)
    {
      rc = ast_exec(parser->ast);
    }
    else
    {
      rc = 2;
    }
    lexer_destroy(lexer);
    parser_free(parser);
    free(input);
    free(new_input);
  
    free(variables_list);
    return rc;
}
