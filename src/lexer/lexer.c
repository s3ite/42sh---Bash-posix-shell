#include "lexer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int in(char c, char *delim) {
  size_t i = 0;
  while (delim[i] != '\0' && delim[i] != c) {
    i++;
  }
  return (delim[i] == c || c == '\0');
}

int is_redirection(char *str) {
  char *redir_list[7] = {">|", "<>", ">>", ">&", "<&", ">", "<"};
  int j = 0;
  while (j < 7) {
    size_t len_r = strlen(redir_list[j]);
    if (strncmp(str, redir_list[j], len_r) == 0) {
      break;
    }
    j++;
  }
  if (j < 7) {
    return j + 1;
  }
  return 0;
}

int get_token_type(char *str) {
  if (strlen(str) == 0) {
    return TOKEN_EOF;
  }
  if (in(str[0], " \t")) {
    return -1;
  }
  char *tok_list[NUM_TOK - 2] = {";",    "|",     "&&",   "||",  "\n",    "if",
                                 "then", "elif",  "else", "fi",  "while", "do",
                                 "done", "until", "!",    "for", "{",     "}",
                                 "(",    ")"}; // TO_UPDATE:token
  size_t i = 0;
  if (is_redirection(str)) {
    return TOKEN_REDIRECTION;
  }
  while (i < NUM_TOK - 2) {
    size_t len = strlen(tok_list[i]);
    if (i < 5) {
      if (strncmp(str, tok_list[i], len) == 0) {
        break;
      }
    } else {
      if (strncmp(str, tok_list[i], len) == 0 &&
          in(str[len], "; ><|\t\n'\"$")) {
        break;
      }
    }
    i++;
  }
  return (i == NUM_TOK - 2) ? NUM_TOK - 1 : i;
}

int handle_comment(char *input) {
  int i = 0;
  while (*(input + i) != '\0' && *(input + i) != '\n') {
    i++;
  }
  return i;
}

char *strnappend(char *str1,char *str2,int n)
{
  if(n==0 || str2 == NULL || str1 == NULL)
  {
    return str1;
  }
  int i = strlen(str1);
  str1=realloc(str1, 1+ i + n);
  int j = 0;
  while(j<n)
  {
    str1[i+j]=str2[j];
    j++;
  }
  str1[i+j]='\0';
  return str1;
}

struct token *handle_quote(char *input) {
  int j = 1;
  while (*(input + j) != '\0' && *(input + j) != '\'') {
    j++;
  }
  if (*(input + j) == '\0') {
    return NULL;
  }
  char *value = malloc(j + 1);
  strncpy(value, input + 1, j - 1);
  if (j > 0) {
    value[j - 1] = '\0';
    int k=0;
    while(!in(*(input + j + k +1),"; ><|\t\n'\"$"))
    {
      k++;
    }
    value=strnappend(value,input+j+1,k);
  }
  struct token *tok = token_init(value, WORD);
  return tok;
}

struct token *handle_word(char *input) {
  int j = 0;
  while (!in(input[j], "; ><|\t\n'\"$")) {
    j++;
  }
  char *value = malloc(j + 1);
  strncpy(value, input, j);
  if (j > 0) {
    value[j] = '\0';
  }
  struct token *tok = token_init(value, WORD);
  return tok;
}

struct lexer *lexer_load(char *input, struct lexer *res) {
  int i = 0;
  while (input[i] != '\0') {
    if (strncmp(input + i, "#", 1) == 0) // gestion des commentaires
    {
      i += handle_comment(input + i);
    } else if (strncmp(input + i, "'", 1) == 0) // getsion des singles quotes
    {
      struct token *tok = handle_quote(input + i);
      if (!tok)
        return NULL;
      if (strlen(tok->value) == 0) {
        free(tok->value);
        free(tok);
        i += 2;
      } else {
        res = lexer_append(res, tok);
        i += strlen(tok->value) + 2;
      }
    } else {
      int tok_type = get_token_type(input + i);
      if (tok_type == WORD) // gestion des words
      {
        struct token *tok = handle_word(input + i);
        res = lexer_append(res, tok);
        i += strlen(tok->value);
      } else if (tok_type == TOKEN_REDIRECTION) // gestion des redirections
      {
        char *redir_list[7] = {">|", "<>", ">>", ">&", "<&", ">", "<"};
        struct token *tok = token_init(
            redir_list[is_redirection(input + i) - 1], TOKEN_REDIRECTION);
        res = lexer_append(res, tok);
        i += strlen(redir_list[is_redirection(input + i) - 1]);
      } else if (tok_type != -1 &&
                 tok_type != TOKEN_EOF) // gestion tokens normaux
      {
        char *tok_list[NUM_TOK - 2] = {
            ";",    "|",    "&&", "||",    "\n", "if",   "then",
            "elif", "else", "fi", "while", "do", "done", "until",
            "!",    "for",  "{",  "}",     "(",  ")"}; // TO_UPDATE:token
        struct token *tok = token_init(tok_list[tok_type], tok_type);
        res = lexer_append(res, tok);
        i += strlen(tok_list[tok_type]);
      } else
        i++;
    }
  }
  struct token *tok = token_init("\0", TOKEN_EOF);
  res = lexer_append(res, tok);
  return res;
}

/*
int main(int argc, char **argv) {
  if (argc) {
    struct lexer *a = lexer_init(10, argv[1]);
    a = lexer_load(argv[1], a);
    if (a)
      lexer_print(a);
    else
      printf("a est NULL\n");
  }
}*/
