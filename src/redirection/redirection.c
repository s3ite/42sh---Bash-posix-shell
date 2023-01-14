#include "redirection.h"
#include "../exec/exec.h"
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>



#define FILE_POS 1

/**
 * Gestion de redirection >, >>,  (>|)
 * command2 == string (path de redirection)
*/
int fd_out(struct redirection_node *rd_node, int io_fd, bool append)
{
    // on vide stdout
    if(io_fd >= 1)
        fflush(stdout);
    
    struct simple_command_node *tmp = rd_node->command2->node;
    char *dest_path = tmp->prefix->head->value;
    int file_fd ;
    if (append)
        file_fd= open(dest_path, O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    else 
        file_fd= open(dest_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (file_fd == -1)
        errx(1, "cannot open file for redirection");
    
    // DUPLICATE io_fd  || sauvegarde de io_fd
    int fd_dup = dup(io_fd);
    if (fd_dup == -1)
    {
        close(file_fd);
        errx(1, "cannot dup io_fd");
    }

    // DUPLICATE Given file descriptor in stdout
    int io_dup = dup2(file_fd, io_fd);  //io_fd pointe sur file_fd
    if (io_dup == -1)
    {
        close(file_fd);
        close(fd_dup);
        errx(1, "cannot dup io_fd");
    }

    //Close given file descriptor, as it is not needed anymore
    if (close(file_fd) == -1)
    {
        close(io_dup);
        close(fd_dup);
        errx(1, "cannot close file");
    }

    // case of >>
    if (append)
        lseek(io_fd, 0, SEEK_END);

    // execute command 1
    ast_exec(rd_node->command1);
    
    // restore io_fd
    io_dup = dup2(fd_dup, io_fd);
    if (io_dup == -1)
    {
        close(fd_dup);
        errx(1, "could not restore STDOUT_FILENO");
    }

    // close stdout duplicate as stdout's file descriptor was retrieved in
    if (close(fd_dup) == -1)
        return -1;
    
    return 0;
}


/**
 * Gestion de redirection <
*/
int fd_in(struct redirection_node *rd_node, int io_fd)
{
    (void) io_fd; 

    char *source_path = rd_node->command2->node;
    char buffer[32000];

    FILE *file_ptr = fopen(source_path, "r");
    if (file_ptr == NULL)
        errx(1, "cannot open file for redirection");  // TODO gestion d'erreur.
    
   fread(buffer, 1, 32000, file_ptr);

    if(fclose(file_ptr) == EOF)
        errx(1, "cannot close file for redirection");
    
    // buffer devient l'arguments d'entree de la command1
    struct simple_command_node *input_command = rd_node->command1->node;

    dlist_push_back(input_command->values, buffer);

    // execute command 1
    ast_exec(rd_node->command1);

    return 0;
}


int fd_dup_in(struct redirection_node *rd_node)
{
    (void) rd_node;
    return 0;
}


int fd_io(struct redirection_node *rd_node, int io_fd)
{
    fd_in(rd_node, io_fd);
    fd_out(rd_node, io_fd, false);
    return 0;
}

/**
 * execute rd node following the type of redirection
 * @input : value of the execution the last commands (input)
*/
int redirection_exec_handler(struct redirection_node *rd_node)   
{
    int io_fd = rd_node->io_number == -1 ? 1 : rd_node->io_number;

    if (rd_node->type == FD_OUT)
        return fd_out(rd_node, io_fd, false);
    
    if (rd_node->type == FD_OUT_APPEND)
        return fd_out(rd_node, io_fd, true);

    if (rd_node->type == FD_IN)
        return fd_in(rd_node, io_fd);

    if (rd_node->type == FD_IO)
        return fd_io(rd_node, io_fd);
    

    
    // TODO
    return -1;
}
