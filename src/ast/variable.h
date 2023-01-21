#ifndef VARIABLE_H
#define VARIABLE_H

#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define VALUE_SIZE 4096
#define NAME_SIZE 1024

extern struct variables_list *variables_list;

enum value_type {
    TYPE_INTEGER,
    TYPE_STRING,
};

union value {
    char *string;
    int integer;
};

struct variable_item
{
    char *name;
    union value value;
    enum value_type type;

    struct variable_item *next;
    struct variable_item *prev;
};

struct variables_list
{
    struct variable_item *head;
    struct variable_item *tail;
    struct variable_item positionnal[100];
    int size;    
};


void free_variables(struct variables_list *list);

struct variables_list *init_variables_list(void);
struct variable_item *init_item(char *name, union value value, enum value_type value_type);
struct variable_item *get_variable(struct variables_list *list, char *name);
int remove_variable(struct variables_list *list, char *name);
char *get_value(struct variable_item *item);
int update_variable(struct variables_list *list, char *name, enum value_type type, union value value);
int add_variable(struct variables_list *list, struct variable_item *var);

// special variables update
int update_interrogation(struct variables_list *list, int last_rc);
int update_random(struct variables_list *list);
int update_oldpwd(struct variables_list *list, char *old_pwd);
int update_pwd(struct variables_list *list, char *new_pwd);
#endif /* ! VARIABLE_H */
