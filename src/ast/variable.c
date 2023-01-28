#define _POSIX_C_SOURCE 200809L

#include "variable.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../others/tools.h"

// variable global pour la gestion des variables
struct variables_list *variables_list;

struct variable_item *init_item(char *name, union value value,
                                enum value_type value_type)
{
    struct variable_item *item = malloc(sizeof(struct variable_item));
    item->name = strdup(name);

    if (value_type == TYPE_STRING)
        item->value.string = strdup(value.string);

    else
        item->value.integer = value.integer;

    item->type = value_type;
    item->next = NULL;
    item->prev = NULL;
    return item;
}

void destroy_item(struct variable_item *item)
{
    free(item->value.string);
    free(item);
}
int update_variable(struct variables_list *list, char *name,
                    enum value_type type, union value value)
{
    if (list == NULL || list->head == NULL)
        return -1;

    struct variable_item *head = list->head;

    for (; head != NULL; head = head->next)
    {
        if (strcmp(head->name, name) == 0)
        {
            if (type == TYPE_STRING)
            {
                free(head->value.string);
                head->value.string = strdup(value.string);
            }
            else
                head->value.integer = value.integer;

            return 0;
        }
    }

    return -1;
}

int update_question_mark(struct variables_list *list, int value)
{
    (void)list;
    return update_variable(variables_list, "?", TYPE_INTEGER,
                           (union value){ .integer = value });
}

/**
 * add a variable to the list of variables
 * @param list the list of variables
 * @param var the struct variable to add or replace
 * @return 0 on succes, -1 on failure
 */
int add_variable(struct variables_list *list, struct variable_item *var)
{
    if (strcmp(var->name, "IFS") == 0)
        destroy_item(var);

    if (list == NULL || var == NULL)
        return -1;

    // if (replace_variable(list, var->name, var->value) == 0)
    // return 0;

    if (list->head == NULL)
        list->head = var;

    if (list->tail != NULL)
        list->tail->next = var;

    // add
    var->prev = list->tail;
    list->tail = var;
    list->size++;

    return 0;
}

struct variable_item *get_variable(struct variables_list *list, char *name)
{
    if (list == NULL)
        return NULL;

    struct variable_item *head = list->head;

    // replace
    for (; head; head = head->next)
    {
        if (strcmp(head->name, name) == 0)
            return head;
    }

    return NULL;
}

// return string formatted value of variable
char *get_value(struct variable_item *item)
{
    return item->value.string;
}

/**
 * remove the variable from the list and resize  the list
 */
int remove_variable(struct variables_list *list, char *name)
{
    struct variable_item *head = list->head;
    for (; head; head = head->next)
    {
        if (strcmp(head->name, name) == 0)
        {
            head->prev->next = head->next;
            head->next->prev = head->prev;
            list->size--;
            free(head);
            return 0;
        }
    }
    return 0; // Error handling
}

void free_variable_list(struct variables_list *list)
{
    struct variable_item *head = list->head;
    struct variable_item *tmp;

    for (; head;)
    {
        tmp = head;
        head = head->next;
        free(tmp->value.string);
        free(tmp);
    }
    free(list);
}

/**
 * Init the variable list structure
 */
struct variables_list *init_variables_list(void)
{
    struct variables_list *list = malloc(sizeof(struct variables_list));
    char *buffer2 = calloc(1, 1024);
    getcwd(buffer2, 1024);
    if (buffer2 == NULL)
        buffer2 = "";

    list->head = NULL;
    list->tail = NULL;

    list->size = 0;

    add_variable(
        list, init_item("0", (union value){ .string = "42sh" }, TYPE_STRING));
    add_variable(list,
                 init_item("?", (union value){ .integer = 0 }, TYPE_INTEGER));
    add_variable(
        list, init_item("OLDPWD", (union value){ .string = "" }, TYPE_STRING));
    add_variable(
        list,
        init_item("PWD", (union value){ .string = buffer2 }, TYPE_STRING));

    free(buffer2);
    return list;
}

void free_variables(void)
{
    struct variable_item *item = variables_list->head;
    struct variable_item *tmp;

    while (item)
    {
        tmp = item;
        item = item->next;
        free(tmp->name);
        if (tmp->type == TYPE_STRING)
            free(tmp->value.string);
        free(tmp);
    }

    free(variables_list);
}
