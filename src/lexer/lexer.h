#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TOK 17 //nombre de token sans le TOKEN_EOF(pour faciliter l'ajout de token)

enum TokenType
{
    TOKEN_IF = 0,
    TOKEN_THEN = 1,
    TOKEN_ELIF = 2,
    TOKEN_ELSE = 3,
    TOKEN_FI = 4,
    TOKEN_NEWLINE = 5,
    TOKEN_WHILE = 6,
    TOKEN_SEMICOLON = 7,
    TOKEN_DO = 8,
    TOKEN_DONE = 9,
    TOKEN_UNTIL = 10,
    TOKEN_NEG = 11,
    TOKEN_FOR = 12,
    TOKEN_AND = 13,
    TOKEN_OR = 14,
    TOKEN_REDIRECTION = 15,
    WORD =16,
    TOKEN_EOF = 17,
    
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
struct lexer *lexer_remove(struct lexer *v, size_t i);
struct token *lexer_pop(struct lexer *v);
struct token *lexer_peek(struct lexer *v);

struct lexer *lexer_load(char *input, struct lexer *res);
struct token *token_init(char *value, enum TokenType type);
void token_free(struct token *token);
#endif /* LEXER.H */
