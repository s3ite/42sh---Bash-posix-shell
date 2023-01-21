#define _POSIX_C_SOURCE 200809L

#include "variable.h"
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>


#include "../others/tools.h"

//variable global pour la gestion des variables
struct variables_list * variables_list;

struct variable_item *init_item(char *name, union value value, enum value_type value_type)
{
    struct variable_item *item = malloc(sizeof(struct variable_item));
    item->name = strdup(name);

    if (value_type == TYPE_STRING)
        item->value.string = strdup(value.string);
    else
    {
        item->value.integer = value.integer;
        item->value.string = malloc(VALUE_SIZE);
    }

    item->type = value_type;
    item->next = NULL;
    item->prev = NULL;

    return item;
}

static int replace_variable(struct variables_list *list, char *name, union value value)
{
    if (list == NULL || list->head == NULL)
        return -1;

    struct variable_item *head = list->head;
    
    //replace
    for (; head != NULL; head = head->next)
    {
        if (strcmp(head->name, name) == 0)
        {
            if(head->type == TYPE_STRING)
            {
                free(head->value.string);
                head->value.string = value.string;
            }
            else
                head->value.integer = value.integer;

            return 0;
        }
    }

    return -1;
}


int update_variable(struct variables_list *list, char *name, enum value_type type, union value value)
{
    if (list == NULL || list->head == NULL)
        return -1;

    struct variable_item *head = list->head;

    for (; head != NULL; head = head->next)
    {
        if (strcmp(head->name, name) == 0)
        {
            if(type == TYPE_STRING)
                head->value.string = strdup(value.string);
            else
                head->value.integer = value.integer;

            return 0;
        }
    }

    return -1;
}

/**
 * add a variable to the list of variables
 * @param list the list of variables
 * @param var the struct variable to add or replace 
 * @return 0 on succes, -1 on failure
 */
int add_variable(struct variables_list *list, struct variable_item *var)
{
    if (list == NULL || var == NULL)
        return -1;
    
    //if (replace_variable(list, var->name, var->value) == 0)
        //return 0; 

    if(list->head == NULL)
        list->head = var;

    if(list->tail != NULL)
        list->tail->next = var;

    //add
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
    
    //replace
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
    if(item->type == TYPE_STRING)
        return item->value.string;

     return snprintf(item->value.string, VALUE_SIZE, "%d", item->value.integer) > 0 ? item->value.string : NULL;
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

    return 0; //Error handling
}

void free_variable_list(struct variables_list *list)
{
    struct variable_item *head = list->head;
    struct variable_item *tmp;
    
    for (; head; )
    {
        tmp = head;
        head = head->next;
        free(tmp->value.string);
        free(tmp);
    }
    free(list);
}

//========================Special Functions==========================

/**
 * Special variable $ifs
 */
char *special_ifs(struct variables_list *list)
{
    (void)list;
    return NULL;
}


/**
 * Get the variable a the index n
 */
char *special_position(struct variables_list *list, int index)
{
    (void)list;
    (void)index;
    return NULL;
}

/**
 * Special variable $@
 */
char *special_arobase(struct variables_list *list)
{
    (void)list;
    return NULL;
}

/**
 * Special variable $*
 */
char *special_star(struct variables_list *list)
{
    (void)list;
    return NULL;
}

/**
 * Special variable $#
 */
char *special_hashtag(struct variables_list *list)
{
    (void)list;
    return NULL;
}

/**
 * Special variable $?
 */
int update_interrogation(struct variables_list *list, int last_rc)
{
    return replace_variable(list, "?", (union value) {.integer = last_rc});
}


/**
 * Special variable $RANDOM
 */
int update_random(struct variables_list *list)
{
    union value value = {.integer = rand()};

    return replace_variable(list, "RANDOM", value); 
}

/**
 * Special variable $oldpwd
 */
int update_oldpwd(struct variables_list *list, char *old_pwd)
{
    int res = replace_variable(list, "OLDPWD", (union value) {.string = old_pwd});
    return res;
}

/**
 * Special variable $pwd
 */
int update_pwd(struct variables_list *list, char *new_pwd)
{
    struct variable_item *item = get_variable(list, "PWD");
    if (item == NULL)
        return -1;

    
    int res = update_oldpwd(list, item->value.string);
    if (res != 0)
        return res;

    return replace_variable(list, "PWD", (union value) {.string = new_pwd});
    
}



/**
 * Init the variable list structure
 */
struct variables_list *init_variables_list(void)
{
    struct variables_list *list = malloc(sizeof(struct variables_list));
    list->head = NULL;
    list->tail = NULL;

    list->size = 0;
    
    union value value =  {.integer = getpid()};
    add_variable(list, init_item("$", value, TYPE_INTEGER));

    value.integer = rand();
    add_variable(list, init_item("RANDOM", value,  TYPE_INTEGER));

    value.integer = 0;
    add_variable(list, init_item("?", value, TYPE_INTEGER));
    
    value.string = malloc(VALUE_SIZE);
    value.string  = getcwd(value.string, VALUE_SIZE);

    add_variable(list, init_item("PWD", value, TYPE_STRING));
    add_variable(list, init_item("OLDPWD", value, TYPE_STRING));
    add_variable(list, init_item("HOME", (union value) {.string = getenv("HOME")}, TYPE_STRING));

    add_variable(list, init_item("#", (union value) {.string = ""}, TYPE_STRING));
    add_variable(list, init_item("*", (union value) {.string = ""}, TYPE_STRING));
    
    //liste de tout les arguments
    add_variable(list, init_item("@", (union value) {.string = ""}, TYPE_STRING));

    add_variable(list, init_item("UID", (union value) {.integer = getuid()}, TYPE_INTEGER));
    add_variable(list, init_item("IFS", (union value) {.string = ""}, TYPE_STRING));
    add_variable(list, init_item("0", (union value) {.string = "42sh"}, TYPE_STRING));
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
        free(tmp);
    }

    free(variables_list);
}
