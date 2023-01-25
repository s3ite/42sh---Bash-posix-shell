#include "parser.h"


static struct ast *build_node(struct ast *body)
{
    struct ast *ast = malloc(sizeof(struct ast));
    ast->node_type = FUNCTION;
    ast->node = body;
    return ast;
}
struct ast *parse_func(struct lexer *lexer, struct parser *parser)
{
    struct token *token = lexer_peek(lexer);
    if(token->type != WORD)
        return NULL;
    char *name= token->value;
    if(!name)
        return NULL;

    lexer_pop(lexer); //func () , on pop func
    lexer_pop(lexer); // on pop (
    
    token = lexer_peek(lexer);
    if(token->type != TOKEN_CLOSE_PAR)
        return NULL; // bad grammar
    lexer_pop(lexer);

    token = lexer_peek(lexer);
    while (token->type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
        token = lexer_peek(lexer);
    }

    struct ast *body = parse_shell_command(lexer, parser);
    if(!body)
        return NULL;
    ast_append(parser->nodes, body);
    hash_map_insert(get_functions(), name, body );
    printf("Ajout de la fonction %s\n", name);
    return build_node(body);

}
