/*
    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
        int sz                   = strlen(str);
        new_str->data = malloc(sz + 1);
        nstrcp(new_str->data, str, sz);

        return new_str;
}

EXPR_NUM *create_expr_num(double num) {
        EXPR_NUM *new_num = malloc(sizeof(EXPR_NUM));

        if (!new_num) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_num->_expr_op.expr_t = EXPR_T_NUMBER;
        new_num->data            = num;

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

EXPR_VAR *create_expr_var(char *name) {
        EXPR_VAR *new_var = malloc(sizeof(EXPR_VAR));

        if (!new_var) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }
        new_var->_expr_op.expr_t = EXPR_T_VAR;
        int sz                   = strlen(name);

        new_var->var = malloc(sz + 1);

        nstrcp(new_var->var, name, sz);

        return new_var;
}

void free_expr(EXPR_OP *head) {
        if (!head) return;

        switch (head->expr_t) {
                
        case EXPR_T_BINARY:
                free_expr(AS_BIN_OP_PTR(head)->left);
                free_expr(AS_BIN_OP_PTR(head)->right);
                free(head);
                break;
        case EXPR_T_UNARY:
                free_expr(AS_UNR_OP_PTR(head)->body);
                free(head);
                break;
        case EXPR_T_BOOL:
                free(head);
                break;
        case EXPR_T_NUMBER:
                free(head);
                break;
        case EXPR_T_STRING:
                free(((EXPR_STR *)head)->data);
                free(head);
        default:
                break;
        }
}