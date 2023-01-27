#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "../ast/ast.h"

int redirection_exec_handler(struct redirection_node *rd_node);
void reset_fd(struct ast **prefix, int len);

#endif /* ! REDIRECTION_H */
