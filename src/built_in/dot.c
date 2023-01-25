#include "built_in.h"
#include "../run_program.h"
#include <fcntl.h>
#include <unistd.h>

int my_dot(char **cmd)
{
    if (cmd[2] != NULL)
    {
        fprintf(stderr, "cd : too many arguments");
        return 1;
    }

    char *path = cmd[1];

    int file_fd = open(path, O_RDONLY);

    if (file_fd == -1)
    {
        fprintf(stderr, "can't open the file %s\n", path);
        return 1; // error
    }

    
    char buffer[1024];
    if (read(file_fd, buffer, sizeof(buffer)) == -1)
    {
        fprintf(stderr, "error reading the file : my_dot");
        return 1;
    }
    close(file_fd);
    return run_program(buffer);
}
