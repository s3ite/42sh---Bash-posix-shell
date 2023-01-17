#include "lexer.h"

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