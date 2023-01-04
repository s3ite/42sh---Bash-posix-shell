#include 'parser.h'

struct ast_node_while *ast_node_while_attach(struct ast_node **res)
{
    // depending on the ast implementation
    struct ast_node_while *node = ast_node_while_alloc();
    *res = &node->base;
    return node;

    // OR

    *res = ast_node_alloc();
    (*res)->type = AST_WHILE;
    return &(*res)->data.while_data;
}

enum parser_status parse_rule_while(struct ast_node **result, struct lexer *lexer)
{
    int rc;
    // is the next token isn't a while token, the rule doesn't match
    if (!parser_consume(lexer, TOKEN_WHILE))
        return PARSER_NO_MATCH;

    // create a new AST node, and attach it to the result pointer
    struct ast_node_while *while_node = ast_node_while_attach(result);

    // parse the condition, and return the error is a failure occurs
    if ((parse_compound_list(&while_node->condition, lexer)))
        return rc;

    // return the error if there's no "do"
    if ((rc = parser_consume(lexer, TOKEN_DO)))
        return rc;

    // parse the while body
    if ((rc = parse_compound_list(&while_node->body, lexer)))
        return rc;

    // return the error if there's no "done"
    if ((rc = parser_consume(lexer, TOKEN_DONE)))
        return rc;
    return 0;
}