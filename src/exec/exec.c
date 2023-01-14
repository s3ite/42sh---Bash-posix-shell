#include "exec.h"

int run_command(char **cmd)
{
    pid_t childID = fork();
    if (childID == -1)
        return 0;
    if (!childID)
    {
        execvp(cmd[0], cmd);
        exit(1);
    }
    else
    {
        int status = 0;
        waitpid(childID, &status, 0);
        if (WIFEXITED(status))
        {
            int res = WEXITSTATUS(status);
            return res;
        }
        return 1;
    }
}

static void free_cmd(char **cmd)
{
    size_t index = 0;
    while (cmd[index])
    {
        free(cmd[index]);
        index++;
    }
    free(cmd);
}

static char **to_command(struct dlist *prefix, struct dlist *values)
{
    if (!prefix || !values)
        return NULL;

    size_t size = dlist_size(prefix) + dlist_size(values);

    struct dlist_item *tmp1 = prefix->head;
    struct dlist_item *tmp2 = values->head;

    char **cmd = malloc(sizeof(char *) * (size + 1));
    size_t i = 0;

    for (i = 0; i < size && tmp1; ++i)
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

static size_t get_cmd_size(char **cmd)
{
    size_t size = 0;
    while (cmd[size])
        size++;
    return size;
}

/**
 * run builtin commands
*/
static int run_buildin(char **cmd)
{
    char *name = cmd[0];
    int size = get_cmd_size(cmd);
    if (strcmp("echo", name) == 0)
    {
        my_echo(cmd, size, 0);
        return 1;
    }
    else if (strcmp("exit", name) == 0)
    {
        my_exit();
        return 1;
    }

    return 0;
}

/**
 * return 1 if the command is a builtin command, 0 otherwise
*/
static int is_buildin(char **cmd)
{
    char *name = cmd[0];
    if (strcmp("echo", name) == 0)
        return 1;
    else if (strcmp("exit", name) == 0)
        return 1;
    
    return 0;
}

static int simple_cmd_exec(struct ast *ast)
{
    struct simple_command_node *cmd_nbode = ast->node;
    struct dlist *prefix = cmd_nbode->prefix;
    struct dlist *values = cmd_nbode->values;

    char **cmd = to_command(prefix, values);
    int rc = 0; // run_buildin(cmd);

    if (is_buildin(cmd))
    {
        run_buildin(cmd);
        free_cmd(cmd);
        return 0;
    }
    else
    {
        rc = run_command(cmd);
    }

    free_cmd(cmd);

    return rc;
}

static int exec_op(struct operator_node *op)
{
    int res = 1;
    if (op->left)
        res = ast_exec(op->left);
    if (op->right)
        res = ast_exec(op->right);
    return res;
}

static int shell_cmd_exec(struct shell_command_node *shell)
{
    int rc = 0;
    if (shell->type == IF)
       rc = exec_if(shell);
   else if (shell->type == WU)
    {
        printf("WHILE/n");
       struct condition_wu *wu = shell->node;
       if (wu->type == WHILE)
            rc = exec_w(shell);
        else
            rc = exec_u(shell);
    }
    return rc;
}


/**
 * execute REDIRECTION ast node
 * Return Return code
 */

static int redir_exec(struct redirection_node *redirection)
{
    (void) redirection;
    // execute input command
    // TODO
    printf("successfully executed REDIRECTION");
    // call redirection function 
    // TODO
    
    return 0;
}


int ast_exec(struct ast *node)
{
    int rc = 0;
    if (node->node_type == SIMPLE_COMMAND)
    {
        rc = simple_cmd_exec(node);
    }
    else if (node->node_type == SHELL_COMMAND)
    {
        printf("SHELL COMMANDE\n");
        struct shell_command_node *shell = node->node;
        rc = shell_cmd_exec(shell);
    }
    else if (node->node_type == OPERATOR)
    {
        struct operator_node *op = node->node;
        rc = exec_op(op);
    }
    else if (node->node_type == REDIRECTION)
    {
        struct redirection_node *redir = node->node;
        rc = redir_exec(redir);
    }
    return rc;
}

/*
    IF EXEC
*/

int exec_if(struct shell_command_node *shell)
{
    int rc = 0;
    int condition = 0;
    struct condition_if_node *if_node = shell->node;
    if (if_node->condition_c)
        condition = ast_exec(if_node->condition_c);
    if (if_node->then_action && !condition)
        rc = ast_exec(if_node->then_action);
    if (if_node->else_action && condition)
        rc = ast_exec(if_node->else_action);

    return rc;
}

int exec_w(struct shell_command_node *shell)
{
   
    int body = 0;
    struct condition_wu *wu_node = shell->node;
    while ((ast_exec(wu_node->condition)) == 0)
    {
        body = ast_exec(wu_node->body);
    }

    return body;
}
/*
    UNTIL 
*/

int exec_u(struct shell_command_node *shell)
{
    int body = 0;
    struct condition_wu *wu_node = shell->node;
    while ((ast_exec(wu_node->condition)) != 0)
    {
        body = ast_exec(wu_node->body);
    }

    return body;
}
