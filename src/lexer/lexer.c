#include "lexer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lexer *lexer_init(size_t n, char *input) {
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

void lexer_destroy(struct lexer *v) {
  for (size_t i = 0; i < v->size; i++) {
    if (v->data[i]->type == WORD) {
      free(v->data[i]->value);
    }
    free(v->data[i]);
  }
  // free(v->input);  //mettre en commentaire si l'input du lexer est
  // statique(test)
  free(v->data);
  free(v);
}

struct lexer *lexer_resize(struct lexer *v, size_t n) {
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

  if (n < v->size) {
    v->size = n;
    return v;
  }
  return v;
}

struct lexer *lexer_append(struct lexer *v, struct token *elt) {
  if (v == NULL)
    return v;
  if (v->size > v->capacity - 1)
    v = lexer_resize(v, v->capacity * 2);
  v->data[v->size] = elt;
  v->size += 1;
  return v;
}

void lexer_print(struct lexer *v) {
  printf("input is: %s\n", v->input);
  printf("token list is: ");
  for (size_t i = 0; i < v->size; i++) {
    printf("%s ", v->data[i]->value);
  }
  printf("\ntokentype list is: ");
  for (size_t i = 0; i < v->size; i++) {
    printf("%d ", (enum TokenType)v->data[i]->type);
  }
  printf("\n");
}

static void left_shift(struct token **arr, int index, int size) {
  while (index < size - 1) {
    arr[index] = arr[index + 1];
    index++;
  }
}

struct lexer *lexer_remove(struct lexer *v, size_t i) {
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

struct token *lexer_peek(struct lexer *v) {
  return v->data[v->index];
}

struct token *lexer_pop(struct lexer *v) {
  struct token *ret = v->data[v->index];
  v->index += 1;
  return ret;
}

struct token *token_init(char *value, enum TokenType type) {
  struct token *ret = ret = malloc(sizeof(struct token));
  if (!ret) {
    return NULL;
  }
  ret->type = type;
  ret->value = value;
  return ret;
}

void token_free(struct token *token) {
  if (token) {
    if (token->type == WORD && token->value)
      free(token->value);
    free(token);
  }
}

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
  char *tok_list[NUM_TOK - 2] = {";","|", "if",  "&&",  "||",   "then", "elif", "else", "fi",    "\n",
                                 "while",     "do",   "done", "until", "!",
                                 "for",         "{",     "}",
                                 "(",     ")"}; // TO_UPDATE:token
  size_t i = 0;
  if (is_redirection(str)) {
    return TOKEN_REDIRECTION;
  }
  while (i < NUM_TOK - 2) {
    size_t len = strlen(tok_list[i]);
    if(i<4){
        if (strncmp(str, tok_list[i], len) == 0) {
            break;
        }
    }else{
        if (strncmp(str, tok_list[i], len) == 0 && in(str[len], "; ><|\t\n'")) {
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
  }
  struct token *tok = token_init(value, WORD);
  return tok;
}

struct token *handle_word(char *input) {
  int j = 0;
  while (!in(input[j], "; ><|\t\n'")) {
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
      res = lexer_append(res, tok);
      i += strlen(tok->value) + 2;
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
        char *tok_list[NUM_TOK - 2] = {";","|", "if",  "&&",  "||",   "then", "elif", "else", "fi",    "\n",
                                 "while",     "do",   "done", "until", "!",
                                 "for",         "{",     "}",
                                 "(",     ")"}; // TO_UPDATE:token
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
int main(int argc,char **argv)

{
    if(argc)
    {
        struct lexer *a=lexer_init(10, argv[1]);
        a=lexer_load(argv[1],a);
        if(a)
            lexer_print(a);
        else
            printf("a est NULL\n");
    }
}
*/