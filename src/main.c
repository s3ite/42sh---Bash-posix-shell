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

#include "expansion/expansion.h"
#include "./ast/variable.h"
#include <stdlib.h>




int main(int argc, char **argv)
{
   variables_list = init_variables_list();
    
    char *input = parse_command_line(argc, argv);
    if(!input)
    {
      fprintf(stderr, "The command line syntax is: 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
      return 2;
    }

    // expansion of variable
    // while(contains_variable(input))
    //   input = expand_variable(input, variables_list);

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
  
    free_variables();
    return rc;
}
