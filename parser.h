#ifndef PARSER_H
#define PARSER_H

#include 'ast.h'

enum token_type
{
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_DONE,
};

enum parser_status
{
    PARSER_DONE = 0,
    PARSER_NO_MATCH,
    PARSER_ERROR,
};

#endif /* PARSER_H */