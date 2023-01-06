#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


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
        //free(v->data[i]->value);
        free(v->data[i]);
    }
    free(v->data);
    //free(v->input);
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

void lexer_print(const struct lexer *v)
{
    return;
    /*if (v->size == 0)
    {
        printf("\n");
        return;
    }
    for (size_t i = 0; i < v->size - 1; i++)
    {
        printf("%d,", v->data[i]);
    }
    if (v->size > 0)
        printf("%d", v->data[v->size - 1]);
    printf("\n");*/
}

struct lexer *lexer_reset(struct lexer *v, size_t n)
{
    struct lexer *newVect = lexer_resize(v, n);
    v = newVect;
    v->size = 0;
    return v;
}

static void shift(struct token **arr, int index, int size)
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

struct token *peek(struct lexer *v)
{
    return v->data[v->index];
}

struct token *pop(struct lexer *v)
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

struct lexer *lexer (char *input, struct lexer *res)
{
    int i = 0;
    while(input[i] != '\0')
    {
        if(strncasecmp(input + i,";",1) == 0)
        {
            struct token *tok = token_init(";", TOKEN_SEMICOLON);
            res = lexer_append(res, tok);
            i += 1;
        }
        else if(strncasecmp(input + i,"'",1) == 0)
        {
            struct token *tok = token_init("'", TOKEN_QUOTE);
            res = lexer_append(res, tok);
            i += 1;
        }
        else if(strncasecmp(input + i,"\n",1) == 0)
        {
            struct token *tok = token_init("\n", TOKEN_NEWLINE);
            res = lexer_append(res, tok);
            i += 1;
        }
        else if(strncasecmp(input + i,"if",2) == 0)
        {
            struct token *tok = token_init("if", TOKEN_IF);
            res = lexer_append(res, tok);
            i += 2;
        }
        else if(strncasecmp(input + i,"fi",2) == 0)
        {
            struct token *tok = token_init("fi", TOKEN_FI);
            res = lexer_append(res, tok);
            i += 2;
        }
        else if(strncasecmp(input + i,"then",4) == 0)
        {
            struct token *tok = token_init("then", TOKEN_THEN);
            res = lexer_append(res, tok);
            i += 4;
        }
        else if(strncasecmp(input + i,"else",4) == 0)
        {
            struct token *tok = token_init("else", TOKEN_ELSE);
            res = lexer_append(res, tok);
            i += 4;
        }
        else if(strncasecmp(input + i,"elif",4) == 0)
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
    return res;
}

int main(int argc,char **argv)
{
    struct lexer *a=lexer_init(10, argv[1]);
    a=lexer(argv[1],a);
    /*printf("input is: %s\n",a->input);
    printf("token list is: ");
    for(size_t i = 0; i<a->size;i++)
    {
        printf("%s ",a->data[i]->value);
    }
    printf("\ntokentype list is: ");
    for(size_t i = 0; i<a->size;i++)
    {
        printf("%d ",(enum TokenType) a->data[i]->type);
    }
    printf("\n");*/
    struct token *t = peek(a);
    printf("%s\n",t->value);
    t=peek(a);
    printf("%s\n",t->value);
    t=pop(a);
    printf("%s\n",t->value);
    t=peek(a);
    printf("%s\n",t->value);
    lexer_destroy(a);
}