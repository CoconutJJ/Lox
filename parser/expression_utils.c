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

struct expr_bin_op *create_expr_bin_op(enum op_v op, int line) {
        struct expr_bin_op *new_op = malloc(sizeof(struct expr_bin_op));

        if (!new_op) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_op->_expr_op.expr_t = EXPR_T_BINARY;
        new_op->op = op;
        new_op->left = NULL;
        new_op->right = NULL;
        new_op->_expr_op.line = line;
        return new_op;
}

struct expr_unr_op *create_expr_unr_op(enum op_v op, int line) {
        struct expr_unr_op *new_op = malloc(sizeof(struct expr_unr_op));

        if (!new_op) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_op->body = NULL;
        new_op->_expr_op.expr_t = EXPR_T_UNARY;
        new_op->op = op;
        new_op->_expr_op.line = line;
        return new_op;
}

struct expr_str *create_expr_str(char *str, int line) {
        struct expr_str *new_str = malloc(sizeof(struct expr_str));

        if (!new_str) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_str->_expr_op.expr_t = EXPR_T_STRING;
        int sz = strlen(str);
        new_str->data = malloc(sz + 1);

        if (!new_str->data) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        nstrcp(new_str->data, str, sz);

        new_str->_expr_op.line = line;

        return new_str;
}

struct expr_number *create_expr_num(double num, int line) {
        struct expr_number *new_num = malloc(sizeof(struct expr_number));

        if (!new_num) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_num->_expr_op.expr_t = EXPR_T_NUMBER;
        new_num->data = num;
        new_num->_expr_op.line = line;
        return new_num;
}

struct expr_bool *create_expr_bool(int b, int line) {
        struct expr_bool *new_bool = malloc(sizeof(struct expr_bool));

        if (!new_bool) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_bool->_expr_op.expr_t = EXPR_T_BOOL;
        new_bool->_expr_op.line = line;
        new_bool->data = b;

        return new_bool;
}

struct expr_var *create_expr_var(char *name, int line) {
        struct expr_var *new_var = malloc(sizeof(struct expr_var));

        if (!new_var) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }
        new_var->_expr_op.expr_t = EXPR_T_VAR;
        new_var->_expr_op.line = line;
        int sz = strlen(name);

        new_var->var = malloc(sz + 1);

        if (!new_var->var) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        nstrcp(new_var->var, name, sz);

        return new_var;
}

void free_expr(struct expr_op *head) {
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
                free(((struct expr_str *)head)->data);
                free(head);
        default:
                break;
        }
}