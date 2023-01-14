#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType
{
    TOKEN_IF = 1,
    TOKEN_THEN = 2,
    TOKEN_ELIF = 3,
    TOKEN_ELSE = 4,
    TOKEN_FI = 5,
    TOKEN_NEWLINE = 6,
    TOKEN_QUOTE = 7,
    TOKEN_SEMICOLON = 8,
    WORD = 9,
    TOKEN_EOF = 10,
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

struct lexer *lexer_init(size_t n, char *input);
void lexer_destroy(struct lexer *v);
struct lexer *lexer_resize(struct lexer *v, size_t n);
struct lexer *lexer_append(struct lexer *v, struct token *elt);
void lexer_print(struct lexer *v);
struct lexer *lexer_reset(struct lexer *v, size_t n);
void shift(struct token **arr, int index, int size);
// struct lexer *lexer_insert(struct lexer *v, size_t i, int elt);
struct lexer *lexer_remove(struct lexer *v, size_t i);
struct token *lexer_pop(struct lexer *v);
struct token *lexer_peek(struct lexer *v);

struct lexer *lexer_load(char *input, struct lexer *res);
struct token *token_init(char *value, enum TokenType type);
void token_free(struct token *token);
#endif /* LEXER.H */
