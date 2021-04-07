
#ifndef EXPR_UTIL_H
#define EXPR_UTIL_H

EXPR_BIN_OP *create_expr_bin_op(EXPR_V op);
EXPR_UNR_OP *create_expr_unr_op(EXPR_V op);
EXPR_STR *create_expr_str(char *str);
EXPR_NUM *create_expr_num(double num);
EXPR_BOOL *create_expr_bool(int b);
EXPR_VAR *create_expr_var(char *name);
void free_expr(EXPR_OP *head);
#endif