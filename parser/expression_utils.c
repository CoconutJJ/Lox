
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "expression.h"

EXPR_BIN_OP *create_expr_bin_op(EXPR_V op) {
        EXPR_BIN_OP *new_op = malloc(sizeof(EXPR_BIN_OP));

        if (!new_op) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_op->_expr_op.expr_t = EXPR_T_BINARY;
        new_op->op              = op;
        new_op->left            = NULL;
        new_op->right           = NULL;

        return new_op;
}

EXPR_UNR_OP *create_expr_unr_op(EXPR_V op) {
        EXPR_UNR_OP *new_op = malloc(sizeof(EXPR_UNR_OP));

        if (!new_op) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_op->body            = NULL;
        new_op->_expr_op.expr_t = EXPR_T_UNARY;
        new_op->op              = op;
        return new_op;
}

EXPR_STR *create_expr_str(char *str) {
        EXPR_STR *new_str = malloc(sizeof(EXPR_STR));

        if (!new_str) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_str->_expr_op.expr_t = EXPR_T_STRING;
        new_str->data            = malloc(sizeof(EXPR_STR));
        int sz                   = strlen(str);
        nstrcp(new_str->data, str, sz);

        return new_str;
}

EXPR_NUM *create_expr_num(char *num) {
        EXPR_NUM *new_num = malloc(sizeof(EXPR_NUM));

        if (!new_num) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_num->_expr_op.expr_t = EXPR_T_NUMBER;
        new_num->data            = strtod(num, NULL);

        return new_num;
}

EXPR_BOOL *create_expr_bool(int b) {
        EXPR_BOOL *new_bool = malloc(sizeof(EXPR_BOOL));

        if (!new_bool) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_bool->_expr_op.expr_t = EXPR_T_BOOL;

        new_bool->data = b;

        return new_bool;
}