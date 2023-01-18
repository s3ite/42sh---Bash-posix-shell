#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TOK 23 // TO_UPDATE:token

enum TokenType
{
    TOKEN_SEMICOLON,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_PIPELINE,
    TOKEN_NEG,
    TOKEN_NEWLINE, // ^^^^^^^^token également séparateur^^^^^^^^
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_UNTIL,
    TOKEN_FOR,
    TOKEN_OPEN_ACC,
    TOKEN_CLOSE_ACC,
    TOKEN_OPEN_PAR,
    TOKEN_CLOSE_PAR,
    TOKEN_IN,
    TOKEN_REDIRECTION, // garder ces 3 tokens en dernier
    WORD,
    TOKEN_EOF,

};

struct token
{
    enum TokenType type;
    char *value;
};

struct lexer
{
    char *input;
    struct token **data;
    size_t index;
    size_t capacity;
    size_t size;
};

// from lexer_lib.c
struct lexer *lexer_init(size_t n, char *input);
void lexer_destroy(struct lexer *v);
struct lexer *lexer_resize(struct lexer *v, size_t n);
struct lexer *lexer_append(struct lexer *v, struct token *elt);
void lexer_print(struct lexer *v);
struct token *lexer_pop(struct lexer *v);
struct token *lexer_peek(struct lexer *v);
struct token *token_init(char *value, enum TokenType type);
void token_free(struct token *token);

// from lexer.c
struct lexer *lexer_load(char *input, struct lexer *res);

#endif /* LEXER.H */
