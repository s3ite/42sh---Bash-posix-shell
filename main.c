#include "parser/parser.h"
#include "lexer/lexer.h"

int main(int argc, char **argv)
{

    char *input = argv[1];
    struct lexer *a=lexer_init(10, argv[1]);
    a=lexer_load(argv[1],a);
    parse( a);



    return 1;
}