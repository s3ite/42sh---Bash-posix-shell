#include "parser/parser.h"
#include "lexer/lexer.h"
#include "exec/exec.h"


int main(int argc, char **argv)
{
    (void) argc;
    //char *input = argv[1];
    struct lexer *a=lexer_init(10, argv[1]);
    a=lexer_load(argv[1],a);
    int rc =parse(a);

    if(rc == RC_ERROR)
        return RC_ERROR;
    lexer_destroy(a);
  
    return 1;
}