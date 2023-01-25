#include "lexer.h"

int reduce1(char *input,struct lexer *res, int i)
{
    struct token *tok = handle_word(input + i);
    res = lexer_append(res, tok);
    i += strlen(tok->value);
    tok->value = remove_backslash(tok->value);
    return i;
}

int reduce2(char *input,struct lexer *res, int i)
{
    struct token *tok = handle_quote(input + i);
    if (!tok)
    {
        lexer_destroy(res);
        return -1;
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
    return i;
}

int reduce3(char *input,struct lexer *res, int i)
{
    struct token *tok = handle_var(input + i + 1);
    if(tok==NULL)
    {
        return -1;
    }
    res = lexer_append(res, tok);
    i += strlen(tok->value);
    tok->value = remove_backslash(tok->value);
    return i;
}

int reduce4(char *input,struct lexer *res, int i)
{
    int inc = handle_double_quote(input + i, res) + 1;
    if (inc == 0)
    {
        lexer_destroy(res);
        return -1;
    }
    i += inc;
    return i;
}

int reduce5_1(char *input,struct lexer *res, int i) // gestion des redirections
{
    char *redir_list[7] = {
        ">|", "<>", ">>", ">&", "<&", ">", "<"};
    char *value = malloc(1);
    value[0] = '\0';
    while (!(in(input[i], "; |\t\n'\"$(){}")))
    {
        int j = 0;
        while (j < 7)
        {
            size_t len_r = strlen(redir_list[j]);
            if (strncmp(input + i, redir_list[j], len_r) == 0)
            {
                break;
            }
            j++;
        }
        strappendchar(&value, input[i]);
        if (j < 5)
        {
            i++;
            strappendchar(&value, input[i]);
            i++;
            break;
        }
        i++;
    }
    struct token *tok = token_init(value, TOKEN_REDIRECTION);
    res = lexer_append(res, tok);
    return i;
}

int reduce5(char *input,struct lexer *res, int i)
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
        i=reduce5_1(input,res,i);
    }
    else if (tok_type != -1 && tok_type != TOKEN_EOF) // gestion tokens "normaux"
    {
        char *tok_list[NUM_TOK - 3] = {
                    ";",  "||",    "&&", "|",    "!",     "\n",   "{",
                    "}",  "(",     ")",  "if",   "then",  "elif", "else",
                    "fi", "while", "do", "done", "until", "for",  "in"
        }; // TO_UPDATE:token
        struct token *tok = token_init(tok_list[tok_type], tok_type);
        res = lexer_append(res, tok);
        i += strlen(tok_list[tok_type]);
    }
    else
    {
        i++;
    }
    return i;
}

int reduce6(char *input,struct lexer *res, int i)
{
    if (strncmp(input + i, "\\", 1) == 0) // gestion des backslash
        {
            i=reduce1(input,res,i);
        }
        else if (strncmp(input + i, "#", 1) == 0) // gestion des commentaires
        {
            i += handle_comment(input + i);
        }
        else if (strncmp(input + i, "'", 1) == 0) // getsion des single quote
        {
            i=reduce2(input,res,i);
        }
        else if (strncmp(input + i, "$(", 2) == 0) // getsion des variables
        {
            i=reduce3(input,res,i);
        }
        else if (strncmp(input + i, "\"", 1) == 0) // getsion des double quote
        {
            i=reduce4(input,res,i);
        }
        else
        {
            i=reduce5(input,res,i);
        }
        if (i==-1)
        {
            return -1;
        }
    return i;
}

struct lexer *lexer_load(char *input, struct lexer *res)
{
    int i = 0;
    while (input[i] != '\0')
    {
        i=reduce6(input,res,i);
        if (i==-1)
        {
            return NULL;
        }
    }
    struct token *tok = token_init("\0", TOKEN_EOF); // gestion du token EOF
    res = lexer_append(res, tok);
    return res;
}
