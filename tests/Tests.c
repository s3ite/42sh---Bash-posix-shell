#include <criterion/criterion.h>
#include "../src/parser/parser.h"
#include "../src/lexer/lexer.h"



Test(simple_command, "Hello, World!") {
 struct lexer *lexer = lexer_init(size_t n, "if echoe condition ;then echo its_true; elif echo second; then echo second_fonctionne; else echo else_clause fi;");
 struct parser *parser = parser_init(lexer);

 struct simple_command_node *actual = parse_simple_commande(lexer, parser);

 cr_expect_eq(actual)
}