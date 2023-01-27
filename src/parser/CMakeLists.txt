
add_library(
    parser parser.c parser.h parse_input.c parse_list.c parse_shell_command.c 
    parse_simple_commande.c parse_rule_if.c parse_rule_else.c parse_rule_elif.c
    parse_redirection.c parse_rule_wu.c parse_prefix.c parse_func.c parse_command.c
    parse_compound_list.c parse_func.c

    parse_command_line.c parse_command_line.h

)
