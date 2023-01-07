#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int execute_simple_command(struct simple_command_node *node) {
  pid_t pid = fork();
  if (pid == 0) {
    // This is the child process
    execvp(node->command, node->arguments);
    // If execvp returns, it means there was an error
    perror("Error executing command");
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    // This is the parent process
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
  } else {
    // There was an error calling fork
    perror("Error calling fork");
    exit(EXIT_FAILURE);
  }
}