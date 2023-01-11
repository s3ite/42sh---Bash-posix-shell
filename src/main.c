#include "parser/parser.h"
#include "lexer/lexer.h"
#include "exec/exec.h"
#include "parse_command_line/parse_command_line.h"
#include "ast/print_ast.h"

int main(int argc, char **argv)
{
    
    char *input = parse_command_line(argc,argv);

    struct lexer *lexer = lexer_init(10, input);
    lexer = lexer_load(input, lexer);

    //representation of the ast
    //print_ast(lexer);

    int rc = parse(lexer);
    
    lexer_destroy(lexer);

    if(rc == RC_ERROR)
        return RC_ERROR;

  
    return 1;
}
