#include "lexer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int in(char c, char *delim)
{
    size_t i = 0;
    while (delim[i] != '\0' && delim[i] != c)
    {
        i++;
    }
    return (delim[i] == c || c == '\0');
}

int is_redirection(char *str)
{
    char *redir_list[7] = { ">|", "<>", ">>", ">&", "<&", ">", "<" };
    int j = 0;
    while (j < 7)
    {
        size_t len_r = strlen(redir_list[j]);
        if (strncmp(str, redir_list[j], len_r) == 0)
        {
            break;
        }
        j++;
    }
    if (j < 7)
    {
        return j + 1;
    }
    return 0;
}

int get_token_type(char *str)
{
    if (strlen(str) == 0)
    {
        return TOKEN_EOF;
    }
    if (in(str[0], " \t"))
    {
        return -1;
    }
    char *tok_list[NUM_TOK - 3] = {
        ";",    "||",   "&&", "|", "!",     "\n",  "{",  "}",     "(",  ")","if",   "then",
        "elif", "else", "fi", "while", "do", "done", "until",
           "for", "in"
    }; // TO_UPDATE:token
    size_t i = 0;
    if (is_redirection(str))
    {
        return TOKEN_REDIRECTION;
    }
    while (i < NUM_TOK - 3)
    {
        size_t len = strlen(tok_list[i]);
        if (i < 10)
        {
            if (strncmp(str, tok_list[i], len) == 0)
            {
                break;
            }
        }
        else
        {
            if (strncmp(str, tok_list[i], len) == 0
                && in(str[len], "; ><|\t\n'\"$(){}"))
            {
                break;
            }
        }
        i++;
    }
    return (i == NUM_TOK - 3) ? NUM_TOK - 2 : i;
}

int handle_comment(char *input)
{
    int i = 0;
    while (*(input + i) != '\0' && *(input + i) != '\n')
    {
        i++;
    }
    if(*(input + i) == '\n')
      i++;
    return i;
}

char *strnappend(char *str1, char *str2, int n)
{
    if (n == 0 || str2 == NULL || str1 == NULL)
    {
        return str1;
    }
    int i = strlen(str1);
    str1 = realloc(str1, 1 + i + n);
    int j = 0;
    while (j < n)
    {
        str1[i + j] = str2[j];
        j++;
    }
    str1[i + j] = '\0';
    return str1;
}

char *strappendchar(char *str1, char c)
{
    int i = strlen(str1);
    str1 = realloc(str1, 2 + i);
    str1[i] = c;
    str1[i + 1] = '\0';
    return str1;
}

char *remove_backslash(char *str)
{
    int i = strlen(str);
    char *ret = malloc(i + 1);
    int j = 0;
    int k = 0;
    while (str[j] != '\0')
    {
        if (str[j] == '\\')
        {
            j++;
            if (str[j] == '\\')
            {
                ret[k] = str[j];
                k++;
                j++;
            }
        }
        else
        {
            ret[k] = str[j];
            k++;
            j++;
        }
    }
    ret[k] = '\0';
    free(str);
    return ret;
}

struct token *handle_quote(char *input)
{
    int j = 1;
    while (*(input + j) != '\0' && *(input + j) != '\'')
    {
        j++;
    }
    if (*(input + j) == '\0')
    {
        return NULL;
    }
    char *value = malloc(j + 1);
    strncpy(value, input + 1, j - 1);
    if (j > 0)
    {
        value[j - 1] = '\0';
        int k = 0;
        while (!in(*(input + j + k + 1), "; ><|\t\n'\"$(){}"))
        {
            k++;
        }
        value = strnappend(value, input + j + 1, k);
    }
    struct token *tok = token_init(value, WORD);
    return tok;
}

struct token *handle_word(char *input)
{
    int j = 0;
    int escaped = (input[j] == '\\');
    while (input[j] != '\0' && !(escaped == 0 && in(input[j], "; ><|\t\n'\"$(){}")))
    {
        if (input[j] == '\\')
        {
            escaped = 1;
        }
        else
        {
            escaped = 0;
        }
        j++;
    }
    char *value = malloc(j + 1);
    strncpy(value, input, j);
    if (j > 0)
    {
        value[j] = '\0';
    }
    struct token *tok = token_init(value, WORD);
    return tok;
}

struct token *handle_var(char *input)
{
    int j = 0;
    int escaped = (input[j] == '\\');
    if(input[j]=='(')
    {
        while (input[j] != '\0' && !(escaped == 0 && input[j]==')'))
        {
            if (input[j] == '\\')
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }
            j++;
        }
        if(input[j]=='\0')
        {
            return NULL;
        }
    }
    else
    {
        while (input[j] != '\0' && !(escaped == 0 && in(input[j], "; ><|\t\n'\"$")))
        {
            if (input[j] == '\\')
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }
            j++;
        }
    }
    char *value = malloc(j + 1);
    strncpy(value, input, j);
    value[j] = '\0';
    struct token *tok = token_init(value, TOKEN_VAR);
    return tok;
}

int handle_double_quote(char *input,struct lexer *res)
{
    int j = 1;
    int escaped = (input[j] == '\\');
    int var=(input[j]=='$');
    while (input[j] != '\0' && !(escaped == 0 && input[j]=='"'))
    {
        if(!var)
        {
            char *word=malloc(1);
            word[0]='\0';
            while (input[j] != '\0' && !(escaped == 0 && input[j]=='"') && !var)
            {
                if (input[j] == '\\')
                {
                    j++;
                    if (input[j] == '\\')
                    {
                        word=strappendchar(word,'\\');
                    }
                    else if (input[j]=='$')
                    {
                        word=strappendchar(word,'$');
                    }
                    else if (input[j]=='\n')
                    {
                        word=strappendchar(word,'\n');
                    }
                    else if (input[j]=='"')
                    {
                        word=strappendchar(word,'"');
                    }
                    else
                    {
                        word=strappendchar(word,'\\');
                        word=strappendchar(word,input[j]);
                    }
                }
                else if (input[j] == '$')
                {
                    var=1;
                }
                else
                {
                    word=strappendchar(word,input[j]);
                }
                j++;
            }
            struct token *tok = token_init(word, WORD);
            res=lexer_append(res,tok);
        }
        else
        {
            char *word=malloc(1);
            while (input[j] != '\0' && !(escaped == 0 && input[j]=='"') && var)
            {
                if(in(input[j],"; ><|\t\n'\"$"))
                {
                    var=0;
                }
                else
                {
                    word=strappendchar(word,input[j]);
                }
                j++;
            }
            struct token *tok = token_init(word, TOKEN_VAR);
            res=lexer_append(res,tok);
        }
    }
    if(input[j]=='\0')
    {
        return -1;
    }
    return j;
}

struct lexer *lexer_load(char *input, struct lexer *res)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (strncmp(input + i, "\\", 1) == 0) // gestion des backslash
        {
            struct token *tok = handle_word(input + i);
            res = lexer_append(res, tok);
            i += strlen(tok->value);
            tok->value = remove_backslash(tok->value);
        }
        else if (strncmp(input + i, "#", 1) == 0) // gestion des commentaires
        {
            i += handle_comment(input + i);
        }
        else if (strncmp(input + i, "'", 1) == 0) // getsion des single quote
        {
            struct token *tok = handle_quote(input + i);
            if (!tok)
            {
                lexer_destroy(res);
                return NULL;
            }
            if (strlen(tok->value) == 0)
            {
                free(tok->value);
                free(tok);
                i += 2;
            }
            else
            {
                res = lexer_append(res, tok);
                i += strlen(tok->value) + 2;
            }
        }
        else if (strncmp(input + i, "$", 1) == 0) // getsion des variables
        {
            struct token *tok = handle_var(input + i + 1);
            res = lexer_append(res, tok);
            i += strlen(tok->value) + 1;
            tok->value = remove_backslash(tok->value);
        }
        else if (strncmp(input + i, "\"", 1) == 0) // getsion des double quote
        {
            int inc = handle_double_quote(input + i,res) + 1;;
            if(inc == 0)
            {
                lexer_destroy(res);
                return NULL;
            }
            i += inc;
        }           
        else
        {
            int tok_type = get_token_type(input + i);
            if (tok_type == WORD) // gestion des words
            {
                struct token *tok = handle_word(input + i);
                res = lexer_append(res, tok);
                i += strlen(tok->value);
                tok->value = remove_backslash(tok->value);
            }
            else if (tok_type == TOKEN_REDIRECTION) // gestion des redirections
            {
                char *redir_list[7] = {
                    ">|", "<>", ">>", ">&", "<&", ">", "<"
                };
                struct token *tok =
                    token_init(redir_list[is_redirection(input + i) - 1],
                               TOKEN_REDIRECTION);
                res = lexer_append(res, tok);
                i += strlen(redir_list[is_redirection(input + i) - 1]);
            }
            else if (tok_type != -1
                     && tok_type != TOKEN_EOF) // gestion tokens "normaux"
            {
                char *tok_list[NUM_TOK - 3] = {
                    ";",    "||",   "&&", "|", "!",     "\n",  "{",  "}",     "(",  ")","if",   "then",
                    "elif", "else", "fi", "while", "do", "done", "until",
                    "for", "in"
            }; // TO_UPDATE:token
                struct token *tok = token_init(tok_list[tok_type], tok_type);
                res = lexer_append(res, tok);
                i += strlen(tok_list[tok_type]);
            }
            else
                i++;
        }
    }
    struct token *tok = token_init("\0", TOKEN_EOF); // gestion du token EOF
    res = lexer_append(res, tok);
    return res;
}
