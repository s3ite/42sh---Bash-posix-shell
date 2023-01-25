#include "lexer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int is_redirection(char *str)
{
    char *redir_list[7] = { ">|", "<>", ">>", ">&", "<&", ">", "<" };
    int i = 0;
    int escaped = str[i] == '\\';
    while (str[i] != '\0' && in(str[i], "0123456789\\><"))
    {
        int j = 0;
        while (j < 7)
        {
            size_t len_r = strlen(redir_list[j]);
            if (!escaped && strncmp(str + i, redir_list[j], len_r) == 0)
            {
                break;
            }
            j++;
        }
        if (j < 7)
        {
            return j + 1;
        }
        escaped = (str[i] == '\\' && !escaped);
        i++;
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
    char *tok_list[NUM_TOK - 3] = { ";",     "||",   "&&",   "|",     "!",
                                    "\n",    "{",    "}",    "(",     ")",
                                    "if",    "then", "elif", "else",  "fi",
                                    "while", "do",   "done", "until", "for",
                                    "in" }; // TO_UPDATE:token
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
    if (*(input + i) == '\n')
        i++;
    return i;
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
        while (!in(*(input + j + k + 1), "; ><|\t\n'\"(){}"))
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
    while (input[j] != '\0'
           && !(escaped == 0 && in(input[j], "; ><|\t\n'\"(){}")))
    {
        escaped=(input[j]=='\\')&&!escaped;
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
    if (input[j] == '(')
    {
        while (input[j] != '\0' && !(escaped == 0 && input[j] == ')'))
        {
            escaped=(input[j]=='\\')&&!escaped;
            j++;
        }
        if (input[j] == '\0')
        {
            return NULL;
        }
    }
    else
    {
        while (input[j] != '\0'
               && !(escaped == 0 && in(input[j], "; ><|\t\n'\"$")))
        {
            escaped=(input[j]=='\\')&&!escaped;
            j++;
        }
    }
    char *value = malloc(j + 1);
    strncpy(value, input, j);
    value[j] = '\0';
    struct token *tok = token_init(value, TOKEN_VAR);
    return tok;
}

int reduce_db1(char *input,char **word ,int j)
{
    j++;
    if (input[j] == '\\')
    {
        strappendchar(word, '\\');
    }
    else if (input[j] == '$')
    {
        strappendchar(word, '$');
    }
    else if (input[j] == '\n')
    {
        strappendchar(word, '\n');
    }
    else if (input[j] == '"')
    {
        strappendchar(word, '"');
    }
    else
    {
        strappendchar(word, '\\');
        strappendchar(word, input[j]);
    }
    return j;
}

int reduce_db2(char *input,struct lexer *res, int j,int *var)
{
    char *word = malloc(1);
    word[0] = '\0';
    while (input[j] != '\0' && !(input[j] == '"')&& !(*var))
    {
        if (input[j] == '\\')
        {
            j=reduce_db1(input, &word,j);
        }
        /*
        else if (input[j] == '$')
        {
            *var = 1;
        }
        */
        else
        {
            strappendchar(&word, input[j]);
        }
        j++;
    }
    struct token *tok = token_init(word, WORD);
    res = lexer_append(res, tok);
    return j;
}

int handle_double_quote(char *input, struct lexer *res)
{
    int j = 1;
    int var = (input[j] == '$');
    while (input[j] != '\0' && !(input[j] == '"'))
    {
        if (!var)
        {
            j=reduce_db2(input,res,j,&var);
        }
        else
        {
            char *word = malloc(1);
            word[0]='\0';
            while (input[j] != '\0' && !(input[j] == '"')
                   && var)
            {
                if (in(input[j], "; ><|\t\n'\"$)"))
                {
                    var = 0;
                }
                else
                {
                    strappendchar(&word, input[j]);
                }
                j++;
            }
            struct token *tok = token_init(word, TOKEN_VAR);
            res = lexer_append(res, tok);
        }
    }
    if (input[j] == '\0')
    {
        return -1;
    }
    return j;
}
