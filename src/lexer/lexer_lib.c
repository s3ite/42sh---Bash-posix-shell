#define _POSIX_C_SOURCE  200809L
#include "lexer.h"
#include <unistd.h>

 #include <fcntl.h>
 
struct lexer *lexer_init(size_t n, char *input)
{
    struct lexer *v = malloc(sizeof(struct lexer));

    struct token **array = malloc(n * sizeof(struct token));
    if (v == NULL || array == NULL)
        return NULL;

    v->capacity = n;
    v->size = 0;
    v->data = array;
    v->input = input;
    v->index = 0;
    return v;
}

void lexer_destroy(struct lexer *v)
{
    for (size_t i = 0; i < v->size; i++)
    {
        if (v->data[i]->type == WORD || v->data[i]->type == TOKEN_VAR
            || v->data[i]->type == TOKEN_REDIRECTION)
        {
            free(v->data[i]->value);
        }
        free(v->data[i]);
    }
    // free(v->input);  //mettre en commentaire si l'input du lexer est
    // statique(test)
    free(v->data);
    free(v);
}

struct lexer *lexer_resize(struct lexer *v, size_t n)
{
    if (n == 0)
        return v;
    if (v == NULL)
        return NULL;
    if (n == v->capacity)
        return v;

    struct token **mem = realloc(v->data, n * sizeof(struct token));
    if (mem == NULL)
        return NULL;
    v->capacity = n;
    v->data = mem;

    if (n < v->size)
    {
        v->size = n;
        return v;
    }
    return v;
}

struct lexer *lexer_append(struct lexer *v, struct token *elt)
{
    if (v == NULL)
        return v;
    if (v->size > v->capacity - 1)
        v = lexer_resize(v, v->capacity * 2);
    v->data[v->size] = elt;
    v->size += 1;
    return v;
}

void lexer_print(struct lexer *v)
{
    char buffer[1024];
    char *tmp = "tests/log";
    int nbwrite = 0;
    remove(tmp);
    int fd = open(tmp, O_CREAT | O_WRONLY, 0777);

    sprintf(buffer, "input is: %s\ntoken list is: ", v->input);

    nbwrite += write(fd, buffer, strlen(buffer));
   
    for (size_t i = 0; i < v->size; i++)
    {
        sprintf(buffer, "%s ", v->data[i]->value);
        nbwrite += write(fd, buffer, strlen(buffer));
    }

    nbwrite += write(fd, "\ntokentype list is: ", 21);
    for (size_t i = 0; i < v->size; i++)
    {
        sprintf(buffer, "%d ", (enum TokenType)v->data[i]->type);
        nbwrite += write(fd, buffer, strlen(buffer) );
    }
    close(fd);
    
}

struct token *lexer_peek_two(struct lexer *v)
{
    return v->data[v->index+1];
}

struct token *lexer_peek(struct lexer *v)
{
    return v->data[v->index];
}

struct token *lexer_pop(struct lexer *v)
{
    struct token *ret = v->data[v->index];
    v->index += 1;
    return ret;
}

struct token *token_init(char *value, enum TokenType type)
{
    struct token *ret = ret = malloc(sizeof(struct token));
    if (!ret)
    {
        return NULL;
    }
    ret->type = type;
    ret->value = value;
    return ret;
}

void token_free(struct token *token)
{
    if (token)
    {
        if (token->type == WORD && token->value)
            free(token->value);
        free(token);
    }
}
