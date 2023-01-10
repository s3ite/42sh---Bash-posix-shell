#include "exec.h"


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

static void free_cmd(char **cmd)
{
    size_t index = 0;
    while(cmd[index])
    {
        free(cmd[index]);
        index++;
    }
    free(cmd);
}

static char **to_command(struct dlist *prefix,struct dlist *values)
{
     if (!prefix || !values)
        return NULL;

    size_t size = dlist_size(prefix) + dlist_size(values);

    struct dlist_item *tmp1 = prefix->head;
    struct dlist_item *tmp2 = values->head;

    char **cmd = malloc(sizeof(char *) * (size + 1));
    size_t i = 0;

    for ( i=0; i < size && tmp1; ++i)
    {
        cmd[i] = strdup(tmp1->value);
        tmp1 = tmp1->next;
    }

    for (; i < size && tmp2; ++i)
    {
        cmd[i] = strdup(tmp2->value);
        tmp2 = tmp2->next;
    }

    cmd[size] = NULL;
    return cmd;

}

static int simple_cmd_exec(struct ast *ast)
{

    struct simple_command_node *cmd_nbode = ast->node;
   
    struct dlist *prefix = cmd_nbode->prefix;
    struct dlist *values = cmd_nbode->values;

    //struct dlist_item *head = prefix->head; 
    char **cmd = to_command(prefix, values);

    int rc = run_command(cmd);
    free_cmd(cmd);

    return rc;
}


static int ast_exec(struct ast *node)
{
     if(node && node->node_type == SIMPLE_COMMAND)
            simple_cmd_exec(node);
      return RC_SUCCESS;
}

int global_exec(struct parser *parser)
{
    struct ast *root = parser->ast;
    ast_exec(root);
    struct ast_node *nodes = parser->nodes;
    while(nodes)
    {
      ast_exec(nodes->ast);
      nodes = nodes->next;
    }

    return RC_SUCCESS;
}

