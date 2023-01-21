#include "exec.h"
#include <stdlib.h>
#include "../redirection/redirection.h"
#include "../ast/variable.h"
#include "../expansion/expansion.h"


int run_command(char **cmd)
{
    int status;
    pid_t childID = fork();
    if (childID == -1)
        return 0;
    if (!childID)
    {
        execvp(cmd[0], cmd);
        if (errno == EACCES)
            err(126, "Error while executing cmd: %s", cmd[0]);
        else if (errno == ENOENT)
        {
            err(127, "Error while executing cmd (unknown): %s", cmd[0]);
        }
        else
            err(0, "Error while executing cmd: %s", cmd[0]);
    }
    else
    {
        if (waitpid(childID, &status, 0) != childID)
            return -1;
    }

    return WEXITSTATUS(status);
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

static char **to_command(struct dlist *args, struct dlist *values)
{
    if (!args || !values)
        return NULL;

    size_t size = dlist_size(args) + dlist_size(values);

    struct dlist_item *tmp1 = args->head;
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
        // expansion des variables
        while(contains_variable(cmd[i]))
            cmd[i] = expand_variable(cmd[i], variables_list);

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
        return my_echo(cmd, size, 0);
    }
    else if (strcmp("exit", name) == 0)
    {
        return my_exit(cmd);
    }
    else if (strcmp("true", name) == 0)
    {
        return my_true();
    }
    else if (strcmp("false", name) == 0)
    {
        return my_false();
    }
    else if (strcmp("cd", name) == 0)
    {
        return my_cd(cmd);
    }
    else if (strcmp("unset", name) == 0)
    {
        return my_unset(cmd);
    }

    return 0;
}

/**
 * return 1 if the command is a builtin command, 0 otherwise
 */
static int is_builtin(char **cmd)
{
    char *name = cmd[0];
    if (strcmp("echo", name) == 0)
        return 1;
    else if (strcmp("exit", name) == 0)
        return 1;
    else if (strcmp("true", name) == 0)
        return 1;
    else if (strcmp("false", name) == 0)
        return 1;
    else if (strcmp("cd", name) == 0)
        return 1;

    return 0;
}
/*
// expand simple command variable
struct ast *expand_sp_variable(struct ast *ast)
{
    enum node_type node_type = ast->node_type;
    // expand command and arguments
    
    // expand value
    if (node_type == SIMPLE_COMMAND)
    {
        struct simple_command_node *simple_cmd = ast->node;

        // si pas de value
        if (simple_cmd->values == NULL)
            return ast;
        
        struct dlist_items *values = simple_cmd->values->head;
        //ON boucle sur les arguments et on remplace les valeurs a expandre par les valeurs des variables
        while(values != NULL)
        {
            if (values->values)
        }
    }
}


*/


bool is_variable_assigment(struct dlist *args)
{
    if (args->head->next != NULL)
        return false;
    
    return strchr(args->head->value, '=') != NULL;
}



static int simple_cmd_exec(struct ast *ast)
{

    //ast = expand_variable(ast);

    int rc = 0;
    struct simple_command_node *cmd_nbode = ast->node;
    struct dlist *args = cmd_nbode->args;
    struct dlist *values = cmd_nbode->values;

    struct ast *prefix = cmd_nbode->prefix;
    if (prefix && prefix->node_type == REDIRECTION)
    {
        struct redirection_node *rd_node = prefix->node;
        rc = redirection_exec_handler(rd_node);

        if (rc != 0) // Error handling
            return rc;
    }

    if(is_variable_assigment(args))
    {
        if (values->head != NULL)
            return 127; // Error handling : command not found

        char *name = strdup(strtok(args->head->value, "="));
        char *value = strdup(strtok(NULL, "\0\n\t\r"));

        union value value_var = {.string = value};
        enum value_type value_type = TYPE_STRING;

        // if is an integer value
        for (size_t i = 0; i < strlen(value); i++)
        {
            if (isalnum(value[i]) == 0)
                continue;
            
            add_variable(variables_list, init_item(name, value_var, value_type));
            return 0;
        }
        value_type = TYPE_INTEGER;
        value_var.integer = atoi(value);
        
        add_variable(variables_list, init_item(name, value_var, value_type));
        return 0;
    }

    char **cmd = to_command(args, values);
    if (is_builtin(cmd))
    {
        rc = run_buildin(cmd);
        free_cmd(cmd);
        return rc;
    }
    else
    {
        rc = run_command(cmd);
    }

    free_cmd(cmd);

    return rc;
}

int exec_pipe(struct operator_node *node, int *res)
{
    int fd[2];
    if (pipe(fd) == -1)
        return -1;

    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return -1;
    }

    if (pid1 == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        *res = ast_exec(node->left);
        close(fd[1]);
        exit(*res);
    }
    else
    {
        close(fd[1]);
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            close(fd[0]);
            return -1;
        }
        if (pid2 == 0)
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            *res = ast_exec(node->right);
            close(fd[0]);
            exit(*res);
        }
        else
        {
            close(fd[0]);
            int status1, status2;
            waitpid(pid1, &status1, 0);
            waitpid(pid2, &status2, 0);
            if (WIFEXITED(status2))
            {
                *res = WEXITSTATUS(status2);
                return 0;
            }
            return -1;
        }
    }
}

static int exec_op(struct operator_node *op)
{
    if (op->type == SEMICOLON)
    {
        int res = 1;
        if (op->left)
            res = ast_exec(op->left);
        if (op->right)
            res = ast_exec(op->right);
        return res;
    }
    else if (op->type == NEG)
    {
        if (op->left)
        {
            return !ast_exec(op->left);
        }
        if (op->right)
        {
            return !ast_exec(op->right);
        }
        return 1;
    }
    else if (op->type == OR)
    {
        int left = 0;
        int right = 0;
        if (op->left)
            left = ast_exec(op->left);
        if (!left)
            return left;
        if (op->right)
            right = ast_exec(op->right);
        return right;
    }
    else if (op->type == AND)
    {
        if (!op->left || !op->right)
            return 1;
        int left = ast_exec(op->left);
        if (left)
            return left;
        return ast_exec(op->right);
    }
    else if (op->type == PIPE)
    {
        int res = 0;
        exec_pipe(op, &res);
        return res;
    }
    return 0;
}

static int shell_cmd_exec(struct shell_command_node *shell)
{
    int rc = 0;
    if (shell->type == IF)
        rc = exec_if(shell);
    else if (shell->type == WU)
    {
        struct condition_wu *wu = shell->node;
        if (wu->type == WHILE)
            rc = exec_w(shell);
        else
            rc = exec_u(shell);
    }
    else if (shell->type == BLOCK)
    {
        struct block_node *block = shell->node;
        struct ast *ast = block->ast;
        rc = ast_exec(ast);
    }

    return rc;
}

int ast_exec(struct ast *node)
{

    int rc = 0;

    if (!node)
        return 0;

    if (node->node_type == SIMPLE_COMMAND)
    {
        rc = simple_cmd_exec(node);
    }
    else if (node->node_type == SHELL_COMMAND)
    {
        struct shell_command_node *shell = node->node;
        rc = shell_cmd_exec(shell);
    }
    else if (node->node_type == OPERATOR)
    {
        struct operator_node *op = node->node;
        rc = exec_op(op);
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
