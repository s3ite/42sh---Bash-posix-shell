#ifndef VARIABLE_H
#define VARIABLE_H

enum value_type
{
    INTEGER,
    FLOAT,
    STRING,
};

union data
{
    int entier;
    float decimal;
    char *string;
};

struct value
{
    union data data;
    enum value_type type;
};

struct variable_item
{
    char *name;
    struct value value;
    enum value_type type;
    int index;
};

struct variables_list
{
    struct variable_item *items;
    int size;
    char *last_arg_prev_command;
};

struct variables_list *init_variables_list(void);

int add_variable(struct variables_list *list, char *name, struct value value);

int remove_variable(struct variables_list *list, char *name);
void free_variable_list(struct variables_list *list);
int get_number_items(struct variables_list *variables_list);

int replace_variable(struct variables_list *list, char *name,
                     struct value value);
void shift_variable(struct variables_list *list, int index);
int get_index(struct variables_list *list, char *name);

#endif /* ! VARIABLE_H */