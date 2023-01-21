#ifndef VARIABLE_H
#define VARIABLE_H

#define VALUE_SIZE 4096
#define NAME_SIZE 1024

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
    int size;    
};


void free_variables(struct variables_list *list);

struct variables_list *init_variables_list(void);
struct variable_item *get_variable(struct variables_list *list, char *name);
int remove_variable(struct variables_list *list, char *name);


// special variables update
int update_interrogation(struct variables_list *list, int last_rc);
int update_random(struct variables_list *list);
int update_oldpwd(struct variables_list *list, char *old_pwd);
int update_pwd(struct variables_list *list, char *new_pwd);
#endif /* ! VARIABLE_H */
