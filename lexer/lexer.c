#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "lexer.h"


struct lexer *lexer_init(size_t n, char *input)
{
    struct lexer *v = malloc(sizeof(struct lexer));

    struct token **array = malloc(n * sizeof(struct token));
    if (v == NULL || array == NULL)
        return NULL;

    v->capacity = n;
    v->size = 0;
    v->data = array;
    v->input=input;
    v->index=0;
    return v;
}

void lexer_destroy(struct lexer *v)
{
    for(size_t i = 0; i<v->size;i++)
    {
        if(v->data[i]->type == WORD)
        {
            free(v->data[i]->value);
        }
        free(v->data[i]);
    }
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
    printf("input is: %s\n",v->input);
    printf("token list is: ");
    for(size_t i = 0; i<v->size;i++)
    {
        printf("%s ",v->data[i]->value);
    }
    printf("\ntokentype list is: ");
    for(size_t i = 0; i<v->size;i++)
    {
        printf("%d ",(enum TokenType) v->data[i]->type);
    }
    printf("\n");
}

struct lexer *lexer_reset(struct lexer *v, size_t n)
{
    struct lexer *newVect = lexer_resize(v, n);
    v = newVect;
    v->size = 0;
    return v;
}

void shift(struct token **arr, int index, int size)
{
    while (size > index - 1)
    {
        arr[size + 1] = arr[size];
        size--;
    }
}

/*struct lexer *lexer_insert(struct lexer *v, size_t i, int elt)
{
    if (v == NULL)
        return NULL;
    if (i > v->size)
        return NULL;
    if (i == v->size)
        return lexer_append(v, elt);
    if (v->size >= v->capacity)
    {
        struct lexer *tmp = lexer_resize(v, v->capacity * 2);
        if (!tmp)
            return NULL;
        v = tmp;
    }
    shift(v->data, i, v->size);
    v->data[i] = elt;
    v->size++;
    return v;
}*/

static void left_shift(struct token **arr, int index, int size)
{
    while (index < size - 1)
    {
        arr[index] = arr[index + 1];
        index++;
    }
}

struct lexer *lexer_remove(struct lexer *v, size_t i)
{
    if (!v)
        return NULL;
    if (i >= v->size)
        return NULL;
    left_shift(v->data, i, v->size);
    v->size--;
    if (v->size <= v->capacity / 2)
        v = lexer_resize(v, v->capacity / 2);
    return v;
}

struct token *lexer_peek(struct lexer *v)
{
    return v->data[v->index];
}

struct token *lexer_pop(struct lexer *v)
{
    struct token *ret = v->data[v->index];
    v->index+=1;
    return ret;
}

struct token *token_init(char *value, enum TokenType type)
{
    struct token *ret = ret = malloc(sizeof(struct token));
    if(!ret)
    {
        return NULL;
    }
    ret->type = type;
    ret->value = value;
    return ret;
}

int in(char c, char *delim)
{
    size_t i = 0;
    while(delim[i]!='\0' && delim[i]!=c)
    {
        i++;
    }
    return (delim[i]==c || c=='\0');
}

struct lexer *lexer (char *input, struct lexer *res)
{
    int i = 0;
    while(input[i] != '\0')
    {
        if(strncmp(input + i,"#",1) == 0)
        {
            while(*(input + i) != '\0' && *(input + i) != '\n')
            {
                i++;
            }
        }
        if(strncmp(input + i,";",1) == 0)
        {
            struct token *tok = token_init(";", TOKEN_SEMICOLON);
            res = lexer_append(res, tok);
            i += 1;
        }
        else if(strncmp(input + i,"'",1) == 0)
        {
            int j = 1;
            while(*(input + i + j) != '\0' && *(input + i + j) != '\'')
            {
                j++;
            }
            if(*(input + i + j) == '\0')
            {
                return NULL;
            }
            char *value = malloc(j + 1);
            strncpy(value, input + i + 1, j - 1);
            if(j > 0)
            {
                value[j - 1] = '\0';
            }
            struct token *tok = token_init(value, WORD);
            res = lexer_append(res, tok);
            i += j + 1;
        }
        else if(strncmp(input + i,"\n",1) == 0)
        {
            struct token *tok = token_init("\n", TOKEN_NEWLINE);
            res = lexer_append(res, tok);
            i += 1;
        }
        else if(strncmp(input + i,"if",2) == 0  && in(input[i+2]," \t\n;"))
        {
            struct token *tok = token_init("if", TOKEN_IF);
            res = lexer_append(res, tok);
            i += 2;
        }
        else if(strncmp(input + i,"fi",2) == 0  && in(input[i+2]," \t\n;"))
        {
            struct token *tok = token_init("fi", TOKEN_FI);
            res = lexer_append(res, tok);
            i += 2;
        }
        else if(strncmp(input + i,"then",4) == 0  && in(input[i+4]," \t\n;"))
        {
            struct token *tok = token_init("then", TOKEN_THEN);
            res = lexer_append(res, tok);
            i += 4;
        }
        else if(strncmp(input + i,"else",4) == 0  && in(input[i+4]," \t\n;"))
        {
            struct token *tok = token_init("else", TOKEN_ELSE);
            res = lexer_append(res, tok);
            i += 4;
        }
        else if(strncmp(input + i,"elif",4) == 0  && in(input[i+4]," \t\n;"))
        {
            struct token *tok = token_init("elif", TOKEN_ELIF);
            res = lexer_append(res, tok);
            i += 4;
        }
        else if(*(input + i) != ' ' && *(input + i) != '\t')
        {
            int j = 0;
            while(*(input + i + j) != ';' && *(input + i + j) != '\n' && *(input + i + j) != ' ' && *(input + i + j) != '\0')
            {
                j++;
            }
            char *value = malloc(j + 1);
            strncpy(value, input + i, j);
            if(j > 0)
            {
                value[j] = '\0';
            }
            struct token *tok = token_init(value, WORD);
            res = lexer_append(res, tok);
            i += j;
        }
        else
        {
            i++;
        }
    }
    struct token *tok = token_init("\0", TOKEN_EOF);
    res = lexer_append(res, tok);
    return res;
}

/*
int main(int argc,char **argv)
{
    if(argc)
    {
        struct lexer *a=lexer_init(10, argv[1]);
        a=lexer(argv[1],a);
        if(a!=NULL)
        {
            lexer_print(a);
            struct token *t = lexer_peek(a);
            printf("%s\n",t->value);
            t=lexer_peek(a);
            printf("%s\n",t->value);
            t=lexer_pop(a);
            printf("%s\n",t->value);
            t=lexer_peek(a);
            printf("%s\n",t->value);
            lexer_destroy(a);
        }
    }
}*/