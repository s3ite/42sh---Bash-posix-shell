#include "exec/exec.h"
#include "lexer/lexer.h"
#include "parse_command_line/parse_command_line.h"
#include "parser/parser.h"

int main(int argc, char **argv)
{
    char *input = parse_command_line(argc, argv);
    if (!input)
    {
        while (1)
        {
            char *str = malloc(1024);
            printf("42sh$ :");
            fgets(str, 1024, stdin);
            str[strlen(str) - 1] = '\0';

            struct lexer *lexer = lexer_init(10, str);
            lexer = lexer_load(str, lexer);
            lexer_print(lexer);
            int rc = parse(lexer);
            lexer_destroy(lexer);
            if(rc == RC_ERROR)
             return RC_ERROR;
        }
    }

    struct lexer *lexer = lexer_init(10, input);
    lexer = lexer_load(input, lexer);

    // representation of the ast
    // print_ast(lexer);

    int rc = parse(lexer);

    lexer_destroy(lexer);

    if (rc == RC_ERROR)
        return RC_ERROR;

    return 0;
}
