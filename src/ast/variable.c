#include "variable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Init the variable list structure
 */
struct variables_list *init_variables_list(void) {
  struct variables_list *list = malloc(sizeof(struct variables_list));
  list->size = 0;
  list->last_arg_prev_command = "";

  return list;
}

/**
 * Create a new variable and add it to the list of variables
 * @param list the list of variables
 * @param name the name of the variable
 * @param value the value of the variable
 *
 * @return 0 on succes, -1 on failure
 */
int add_variable(struct variables_list *list, char *name, struct value value) {
  // check if the variable already exists
  int index = get_index(list, name);
  if (index != -1) {
    replace_variable(list, name, value);
    return -1;
  }

  assert(index == -1);

  int size = get_number_items(list);
  struct variable_item item = {.name = name, .value = value, .index = size};

  list->items = realloc(list->items, size + 1);
  list->items[size] = item;

  list->size++;
  return 0;
}

/**
 * replace a variable's value and or type
 * @param list the list of variables
 * @param name the name of the variable
 * @param value the value of the variable
 *
 * @return 0 on succes, -1 on failure
 */
int replace_variable(struct variables_list *list, char *name,
                     struct value value) {
  // check if the variable already exists
  int index = get_index(list, name);
  if (index == -1) {
    add_variable(list, name, value);
    return -1;
  }

  assert(index > -1);

  list->items[index].value = value;

  return 0;
}

/**
 * Shift the list to free the last item in the list and freed memory
 */
void shift_variable(struct variables_list *list, int index) {
  for (int i = index; i < list->size; i++) {
    list->items[i] = list->items[i + 1];
    list->items[i].index = i - 1;
  }
}

/**
 * Search the index of the variable with the same name in the list
 * returns the index of the variable if found otherwise returns -1
 */
int get_index(struct variables_list *list, char *name) {
  for (int i = 0; i < list->size; i++) {
    if (strcmp(list->items[i].name, name) == 0)
      return i;
  }

  return -1;
}

/**
 * remove the variable from the list and resize  the list
 */
int remove_variable(struct variables_list *list, char *name) {
  int index = get_index(list, name);
  shift_variable(list, index);

  list->size--;
  list->items = realloc(list->items, list->size);

  return 0;
}

void free_variable_list(struct variables_list *list) { free(list); }

/**
 * Get the number of variables in the list
 */
int get_number_items(struct variables_list *variables_list) {
  return variables_list->size;
}

//========================Special Functions==========================
/**
 * Get the variable a the index n
 */
char *special_position(struct variables_list *list, int index) {
  (void)list;
  (void)index;
  return NULL;
}

/**
 * Special variable $@
 */
char *special_arobase(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $*
 */
char *special_star(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $#
 */
char *special_hashtag(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $?
 */
char *special_interrogation(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $$
 */
char *special_dollar(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $RANDOM
 */
int special_random() { return 0; }

/**
 * Special variable $UID
 */
int special_uid() { return 0; }

/**
 * Special variable $pwd
 */
char *special_pwd(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $oldpwd
 */
char *special_oldpwd(struct variables_list *list) {
  (void)list;
  return NULL;
}

/**
 * Special variable $ifs
 */
char *special_ifs(struct variables_list *list) {
  (void)list;
  return NULL;
}