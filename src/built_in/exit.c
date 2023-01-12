#define _POSIX_SOURCE

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "built_in.h"

void my_exit()
{
    pid_t shell_pid = getpid();
    
    kill(shell_pid, SIGTERM);
}
