#ifndef VARIABLE_H
#define VARIABLE_H

enum value_type{
    INTEGER,
    FLOAT
    STRING,
}; 

union data{
    int entier;
    float decimal;
    char *string;
};

struct value {
    union data data;
    enum value_type type;
};

struct variable_item{
    char *name;
    struct value value;
    enum value_type type;
    int index;
};

struct variables_list {
    struct variable_item *items;
    int size;
    char *last_arg_prev_command;
};

#endif /* ! VARIABLE_H */