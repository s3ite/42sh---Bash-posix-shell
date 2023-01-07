#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int run_command(char **cmd)
{
    pid_t pid = fork();
   if (pid == 0) {
        execvp(cmd[0], cmd);
        perror("Error executing command");
        exit(EXIT_FAILURE);
  } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
  } else {
        perror("Error calling fork");
        exit(EXIT_FAILURE);
  }
}
