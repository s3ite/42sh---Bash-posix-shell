#define _POSIX_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "./ast/variable.h"
#include "built_in/built_in.h"
#include "exec/exec.h"
#include "expansion/expansion.h"
#include "lexer/lexer.h"
#include "others/hash_map.h"
#include "parser/parse_command_line.h"
#include "parser/parser.h"
#include "run_program.h"

int main(int argc, char **argv)
{
    variables_list = init_variables_list();
    init_map();
    char *input = parse_command_line(argc, argv);
    if (!input)
    {
        fprintf(stderr,
                "The command line syntax is: 42sh [OPTIONS] [SCRIPT] "
                "[ARGUMENTS ...]\n");
        return 2;
    }
    int rc = run_program(input);
    free_variables();

    hash_map_free(get_functions());

    return rc;
}
