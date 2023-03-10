#include "run_program.h"

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
#include "parser/parse_command_line.h"
#include "parser/parser.h"

int run_program(char *input)
{
    char *new_input = remove_escaped_newline(input);
    struct lexer *lexer = lexer_init(10, new_input);
    lexer = lexer_load(new_input, lexer);
    if (!lexer)
    {
        free(variables_list);
        free(input);
        free(new_input);
        fprintf(stderr, "%s", "Syntax error: Unterminated quoted string\n");
        return 2; // erreur lors du lexing
    }
    lexer_print(lexer);

    int rc = 0;
    struct parser *parser = parse(lexer);
    if (parser)
        rc = ast_exec(parser->ast);
    else
        rc = 2;

    update_question_mark(variables_list, 0);

    lexer_destroy(lexer);

    parser_free(parser);
    free(input);
    free(new_input);
    return rc;
}
