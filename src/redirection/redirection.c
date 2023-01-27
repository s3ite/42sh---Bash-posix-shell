#include "redirection.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../exec/exec.h"

void reset_fd(struct ast **prefix, int len)
{
    fflush(stdout);
    int i = 0;
    while ( i < len && prefix[i]->node_type == REDIRECTION)
    {
        struct redirection_node *rd_node = prefix[i]->node;
        if (rd_node->save_io_number != -1)
            dup2(rd_node->save_io_number, rd_node->new_value);
        if (rd_node->save_io_number != -1) 
            dup2(rd_node->save_io_number2, rd_node->new_value2);   
        i++;
    }


}

/**
 * Gestion de redirection >, >>,  (>|)
 * command2 == string (path de redirection)
 */
static int fd_out(struct redirection_node *rd_node, bool append, char *path)
{
    int io_fd = rd_node->io_number == -1 ? STDOUT_FILENO : rd_node->io_number;
    rd_node->new_value = io_fd;

    int file_fd = 0;
    if (append)
        file_fd = open(path, O_APPEND | O_WRONLY | O_CREAT, 0777);
    else
        file_fd = open(path, O_CREAT | O_WRONLY, 0777);

    if (file_fd == -1)
        return 1; // error

    // DUPLICATE io_fd  || sauvegarde de io_fd
    int save_stdout = dup(io_fd);
    rd_node->save_io_number = save_stdout;

    if (save_stdout == -1)
    {
        close(file_fd);
        return 1; // error
    }

    // DUPLICATE Given file descriptor in stdout
    int new_out = dup2(file_fd, io_fd);

    if (new_out == -1)
    {
        close(file_fd);
        close(save_stdout);
        return 1; // error
    }

    // Close given file descriptor, as it is not needed anymore
    if (close(file_fd) == -1)
    {
        close(new_out);
        close(save_stdout);
        return 1; // error
    }

    // case of >>
    if (append)
        lseek(STDOUT_FILENO, 0, SEEK_END);
    return 0;
}

/**
 * Gestion de redirection <
 */
static int fd_in(struct redirection_node *rd_node, char *path)
{
    int io_fd = rd_node->io_number == -1 ? STDIN_FILENO : rd_node->io_number;
    rd_node->new_value = io_fd;

    int file_fd = open(path, O_RDONLY);

    if (file_fd == -1)
    {
        fprintf(stderr, "can't open the file %s\n", path);
        return 1; // error
    }

    // DUPLICATE io_fd  || sauvegarde de io_fd
    int save_stdin = dup(io_fd);
    rd_node->save_io_number = save_stdin;

    if (save_stdin == -1)
    {
        close(file_fd);
        return 1; // error
    }

    // DUPLICATE Given file descriptor in stdin
    int new_in = dup2(file_fd, io_fd);

    if (new_in == -1)
    {
        close(file_fd);
        close(save_stdin);
        return 1; // error
    }

    // Close given file descriptor, as it is not needed anymore
    if (close(file_fd) == -1)
    {
        close(new_in);
        close(save_stdin);
        return 1; // error
    }

    return 0;
}

/**
 * Gestion de redirection <& et >&
*/
static int fd_dup_in_out(struct redirection_node *rd_node, char *path, bool input)
{
    int io_fd = input ? 0 : 1;
    io_fd = rd_node->io_number == -1 ? io_fd : rd_node->io_number;
    rd_node->new_value = io_fd;

    // cas du -
    if (strcmp(path, "-") == 0)
        close(io_fd);

    int file_fd = atoi(path);

    // si fd non existant
    if (isatty(file_fd) == 0)
    {
        fprintf(stderr, "fd_dup : Bad file descriptor %d", file_fd); //
        return 1; // ERROR HANDLING : COMMAND NOT FOUND
    }

    int new_io = dup2(file_fd, io_fd);

    if (new_io == -1)
    {
        fprintf(stderr, "Cannot dup stdin/stdout");
        return 1;
    }

    return 0;
}

// <>
static int fd_io(struct redirection_node *rd_node, char *path)
{
    int io_fd = rd_node->io_number == -1 ? STDIN_FILENO : rd_node->io_number;
    rd_node->new_value = io_fd;

    int file_fd = open(path, O_CREAT | O_RDWR, 0777);
    if (file_fd == -1)
    {
        fprintf(stderr, "Error fd_io : ouverture du fichier");
        return 1; // error
    }

    int save_stdin = dup(io_fd);
    rd_node->save_io_number = save_stdin;

    // DUPLICATE Given file descriptor in stdin
    int new_in = dup2(file_fd, io_fd);
    int new_out = dup2(file_fd, io_fd);

    if (new_in == -1 || new_out == -1)
    {
        close(file_fd);
        return 1; // error
    }

    // Close given file descriptor, as it is not needed anymore
    if (close(file_fd) == -1)
    {
        close(new_in);
        return 1; // error
    }

    return 0;
}
/**
 * execute rd node following the type of redirection
 * @input : value of the execution the last commands (input)
 */
int redirection_exec_handler(struct redirection_node *rd_node)
{
    char *path = rd_node->word;
    if (strcmp(path,"") == 0)
    {
        fprintf(stderr, "PARSE ERROR near redirection op : redirection_exec_handler");
        return 2; // ERROR Handling : PARSE ERROR
    }

    fflush(stdout);

    // >
    if (rd_node->type == FD_OUT || rd_node->type == FD_OUT_NO_CLOBBER)
        return fd_out(rd_node, false, path);
    // >>
    if (rd_node->type == FD_OUT_APPEND)
        return fd_out(rd_node, true, path);

    // >&
    if (rd_node->type == FD_DUP_OUT)
        return fd_dup_in_out(rd_node, path, false);

    // <&
    if (rd_node->type == FD_DUP_IN)
        return fd_dup_in_out(rd_node, path, true);

    // <
    if (rd_node->type == FD_IN)
        return fd_in(rd_node, path);
    // <>
    if (rd_node->type == FD_IO)
        return fd_io(rd_node, path);

    fprintf(stderr, "redirection_exec_handler erreur de lexing");
    return 2; // Error Handling :  Syntax Error
}
