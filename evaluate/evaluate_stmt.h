#ifndef EVALUATE_H
#define EVALUATE_H

#include "../types.h";


int is_truthy(EXPR_OP *val);

EXPR_OP *evaluate_expr(EXPR_OP *tree, ENVIRONMENT *env);

#endif