#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VALUE_SIZE 4096
#define NAME_SIZE 1024

extern struct variables_list *variables_list;

enum value_type
{
    TYPE_INTEGER,
    TYPE_STRING,
};

union value
{
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

struct variables_list *init_variables_list(void);
struct variable_item *init_item(char *name, union value value,
                                enum value_type value_type);
struct variable_item *get_variable(struct variables_list *list, char *name);
int remove_variable(struct variables_list *list, char *name);
char *get_value(struct variable_item *item);
int update_variable(struct variables_list *list, char *name,
                    enum value_type type, union value value);
int add_variable(struct variables_list *list, struct variable_item *var);
void free_variables(void);

int update_question_mark(struct variables_list *list, int value);

#endif /* ! VARIABLE_H */
