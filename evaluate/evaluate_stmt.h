#ifndef EVALUATE_STMT_H
#define EVALUATE_STMT_H

#include "../types.h"


int is_truthy(EXPR_OP *val);

void evaluate_statements(STATEMENT* stmt, ENVIRONMENT* env);

#endif