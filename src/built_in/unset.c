#include "built_in.h"
#include "../ast/variable.h"

char *get_mode(char *mode)
{   
    
    if (mode == NULL || mode[0] !=  '-' ) {
        return "-v";
    }
    if (strcmp(mode, "-f") == 0)
        return "f";
    
    return NULL;
}

int my_unset(char **cmd)
{
    char *mode = get_mode(cmd[1]);
    if (mode == NULL) {
        fprintf(stderr, "command not found"); // Error handling: command not found
        return 2;
    }

    if (strcmp(mode, "-v") == 0) 
        for(int i = 0; cmd[i]; i++)
            remove_variable(variables_list, cmd[i]);
    
    return 0;
}
