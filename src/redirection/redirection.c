#include "redirection.h"
#include "../exec/exec.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Gestion de redirection >, >>,  (>|)
 * command2 == string (path de redirection)
 */
int fd_out(struct redirection_node *rd_node, bool append, char *path) {
  
  int io_fd = rd_node->io_number == -1 ? STDOUT_FILENO : rd_node->io_number;

  int file_fd = 0;
  if (append)
    file_fd = open(path, O_APPEND | O_WRONLY | O_CREAT, 0777);
  else
    file_fd = open(path, O_CREAT | O_WRONLY , 0777);

  if (file_fd == -1)
    return 1; // error

  // DUPLICATE io_fd  || sauvegarde de io_fd
  int save_stdout = dup(io_fd);
  if (save_stdout == -1) {
    close(file_fd);
    return 1; // error
  }

  // DUPLICATE Given file descriptor in stdout
  int new_out = dup2(file_fd, io_fd);

  if (new_out == -1) {
    close(file_fd);
    close(save_stdout);
    return 1; // error
  }

  // Close given file descriptor, as it is not needed anymore
  if (close(file_fd) == -1) {
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
int fd_in(struct redirection_node *rd_node, char *path) {
  int io_fd = rd_node->io_number == -1 ? STDIN_FILENO : rd_node->io_number;

  int file_fd = open(path, O_RDONLY);

  if (file_fd == -1)
    return 1; // error

  // DUPLICATE io_fd  || sauvegarde de io_fd
  int save_stdin = dup(io_fd);
  if (save_stdin == -1) {
    close(file_fd);
    return 1; // error
  }

  // DUPLICATE Given file descriptor in stdin
  int new_in = dup2(file_fd, io_fd);

  if (new_in == -1) {
    close(file_fd);
    close(save_stdin);
    return 1; // error
  }

  // Close given file descriptor, as it is not needed anymore
  if (close(file_fd) == -1) {
    close(new_in);
    close(save_stdin);
    return 1; // error
  }

  return 0;
}



int fd_dup_in_out(struct redirection_node *rd_node, char *path, bool input){
  int io_fd = input ? 0 : 1;
  io_fd = rd_node->io_number == -1 ? io_fd : rd_node->io_number;

  // cas du -
  if (strcmp(path, "-"))
    close(io_fd);

  int file_fd = atoi(path);

  // si fd non existant
  if (isatty(file_fd) == 0)
  {
      fprintf(stderr, "Bad file descriptor"); //
      return 1;
  }

  int new_io = 0;
  if(input)
    dup2(io_fd, STDIN_FILENO);
  else
    dup2(io_fd, STDOUT_FILENO);
  
  if (new_io == -1) {
    fprintf(stderr, "Cannot dup stdin/stdout");
    return 1;
  }

  return 0;
}

int fd_io(struct redirection_node *rd_node, char *path)
{
  int io_fd = rd_node->io_number == -1 ? STDIN_FILENO : rd_node->io_number;

  int file_fd = open(path, O_CREAT | O_RDWR, 0777);

  if (file_fd == -1)
    return 1; // error

  // DUPLICATE io_fd  || sauvegarde de io_fd
  int save_stdin = dup(io_fd);
  int save_stdout = dup(STDOUT_FILENO);

  if (save_stdin == -1 || save_stdout == -1) {
    close(file_fd);
    return 1; // error
  }

  // DUPLICATE Given file descriptor in stdin
  int new_in = dup2(file_fd, io_fd);
  int new_out = dup2(file_fd, io_fd);

  if (new_in == -1 || new_out == -1) {
    close(file_fd);
    close(save_stdin);
    close(save_stdout);
    return 1; // error
  }

  // Close given file descriptor, as it is not needed anymore
  if (close(file_fd) == -1) {
    close(new_in);
    close(save_stdin);
    close(save_stdout);
    return 1; // error
  }

  return 0;
}
/**
 * execute rd node following the type of redirection
 * @input : value of the execution the last commands (input)
 */
int redirection_exec_handler(struct redirection_node *rd_node) {
  char *path = rd_node->word;

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
  return 1;
}
