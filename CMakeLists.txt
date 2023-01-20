cmake_minimum_required(VERSION 3.0)
project(42sh VERSION 1.0.0 LANGUAGES C)

include_directories(src)
add_compile_options(-std=c99)
add_compile_options(-pedantic)
add_compile_options(-Werror)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wvla)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#add_compile_options(-g)


add_executable(42sh
src/ast/ast.c
src/ast/ast.h

src/ast/list.c
src/ast/list.h

src/redirection/redirection.c
src/redirection/redirection.h

src/ast/print_ast.c
src/ast/print_ast.h

src/built_in/built_in.h
src/built_in/boolean.c
src/built_in/exit.c
src/built_in/echo.c
src/built_in/cd.c

src/parser/parser.c
src/parser/parser.h
src/parser/parse_input.c
src/parser/parse_list.c
src/parser/parse_shell_command.c
src/parser/parse_simple_commande.c
src/parser/parse_rule_if.c
src/parser/parse_rule_else.c
src/parser/parse_rule_elif.c
src/parser/parse_redirection.c
src/parser/parse_rule_wu.c
src/parser/parse_prefix.c

src/parser/parse_command.c
src/parser/parse_compound_list.c

src/parse_command_line/parse_command_line.h
src/parse_command_line/parse_command_line.c

src/exec/exec.c
src/exec/exec.h

src/lexer/lexer_lib.c
src/lexer/lexer.c
src/lexer/lexer.h

src/others/tools.c
src/others/tools.h

src/main.c

)