#include "parser.h"


struct ast *parse_condition(struct lexer *lexer, struct parser *parser,
                            enum TokenType type)
{

    lexer_pop(lexer);
    struct ast *ast = parse_compound_list(lexer, parser);
    if (!ast)
        errx(999, "empty");
    if (!ast)
        return ast;
    ast_append(parser->nodes, ast);
    return ast;
}


struct condition_wu *build_condition_wu(struct ast *body, struct ast *condition,enum loop loop)
{
    struct condition_wu *ast = malloc(sizeof(struct condition_wu));
    ast->condition = condition;
    ast->body = body;
    ast->enum = loop;

    return ast;
}

struct ast *parse_rule_wu(struct lexer *lexer, struct parser *parser,
                          struct ast *prev_ast) 
{

    struct token *token = lexer_peek(lexer);
    struct ast *cond = NULL;
    enum loop tmp = NULL ;
    enum shell_type type = WU;

    if (token->type == TOKEN_WHILE)
    {
        tmp = WHILE;
        cond = parse_condition(lexer,parser,TOKEN_WHILE)
         
    }
    else if (token->type = TOKEN_UNTIL)
    {
        tmp = UNTIL;
        cond = parse_condition(lexer,parser,TOKEN_UNTIL)
    }

    if (!cond)
        errx(999, "parse_rule_f parse condition null");
    if (!cond)
        return cond;

    struct at *body = parse_condition(lexer,parser, TOKEN_DO)

    if (!body)
        errx(999, "parse_rule_f right null");
    if (!body)
        return body;

    token = lexer_peek(lexer);
    if (token->type != TOKEN_DONE)
        errx(999, "parse_rule_f oken->type != TOKEN_DONE",
               token->type);
    if (token->type != TOKEN_DONE)
        return NULL;
    lexer_pop(lexer);


    struct condition_wu *while_node = build_condition_wu(cond,body,tmp);
    struct shell_command_node *shell_node = prev_ast->node;
    shell_node->node = while_node;
    shell_node->type = type;


    return prev_ast;


}