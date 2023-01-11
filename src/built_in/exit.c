#define _POSIX_SOURCE

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "built_in.h"

static void cleanupData(struct parser *parser)
{
    // Data to clean
    parser_destroy(parser);
}

void my_exit(struct parser *parser)
{
    // on recupere le pid de l'instance actuelle
    pid_t shell_pid = getppid();

    // catch des signaux et cleanup des donnnees / trap
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    cleanupData(parser);

    // on envoie un SIGQUIT au shell
    kill(shell_pid, SIGQUIT);
}
