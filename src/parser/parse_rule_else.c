#include "parser.h"

struct ast *parse_rule_else(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);

    struct ast *ast = NULL;

    if(token->type == TOKEN_ELSE)
    {
        lexer_pop(lexer);
        ast = parse_compound_list(lexer, parser);
        if(!ast)
            return ast;
        ast_append(parser->nodes,ast);
    }
    else if(token->type == TOKEN_ELIF)
    {
        ast = build_shell_command_node(IF);
        ast_append(parser->nodes, ast);
        ast = parse_rule_if(lexer,parser, ast);
        if(!ast)
            return NULL;
    }
    return NULL;

}