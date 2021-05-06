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

#include "../environment/environment.h"
#include "../error/errors.h"
#include "../parser/expression.h"
#include "../parser/expression_utils.h"
#include "../types.h"

/**
 * Evaluates a Number, String or Bool unit expression and decides whether the
 * value is a truthy value or falsey value.
 */
int is_truthy(EXPR_OP *val) {
        switch (val->expr_t) {
        case EXPR_T_NUMBER:;
                EXPR_NUM *num = (EXPR_NUM *)val;

                return num->data != 0.0;
                break;
        case EXPR_T_STRING:;
                EXPR_STR *str = (EXPR_STR *)val;

                return strlen(str->data) != 0;
                break;
        case EXPR_T_BOOL:;

                EXPR_BOOL *bl = (EXPR_BOOL *)val;

                return bl->data;
                break;
        default:
                fprintf(stderr, "bug: is_truthy: recieved invalid expression");
                exit(EXIT_FAILURE);
                break;
        }
}

/**
 * Deep copies a leaf node from the expression tree and returns the new EXPR
 * struct.
 */
EXPR_OP *copy_leaf(EXPR_OP *op) {
        switch (op->expr_t) {
        case EXPR_T_NUMBER:;
                EXPR_NUM *num = (EXPR_NUM *)op;
                return (EXPR_OP *)create_expr_num(num->data, op->line);
                break;
        case EXPR_T_STRING:;
                EXPR_STR *str = (EXPR_STR *)op;
                return (EXPR_OP *)create_expr_str(str->data, op->line);
                break;
        case EXPR_T_BOOL:;
                EXPR_BOOL *bl = (EXPR_BOOL *)op;
                return (EXPR_OP *)create_expr_bool(bl->data, op->line);
                break;
        case EXPR_T_VAR:;
                EXPR_VAR *var = (EXPR_VAR *)op;
                return (EXPR_OP *)create_expr_var(var->var, op->line);
                break;
        default:
                fprintf(stderr, "bug: copy_leaf: recieved invalid enum");
                exit(EXIT_FAILURE);
                break;
        }
}

EXPR_OP *evaluate_expr(EXPR_OP *tree, ENVIRONMENT *env) {
        if (!tree) return NULL;

        EXPR_OP *result;
        int      line = tree->line;
        switch (tree->expr_t) {
        case EXPR_T_BINARY:;  // <-- very important semicolon, do not remove.

                EXPR_BIN_OP *binop = (EXPR_BIN_OP *)tree;

                EXPR_OP *left  = evaluate_expr(binop->left, env);
                EXPR_OP *right = evaluate_expr(binop->right, env);

                switch (binop->op) {
                case EXPR_V_PLUS:

                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_num(
                                    ((EXPR_NUM *)left)->data +
                                        ((EXPR_NUM *)right)->data,
                                    line);

                        } else if (left->expr_t == EXPR_T_STRING &&
                                   right->expr_t == EXPR_T_STRING) {
                                EXPR_STR *left_str = (EXPR_STR *)left;

                                EXPR_STR *right_str = (EXPR_STR *)right;

                                char concat[strlen(left_str->data) +
                                            strlen(right_str->data) + 1];

                                strcpy(concat, left_str->data);
                                strcat(concat, right_str->data);

                                result =
                                    (EXPR_OP *)create_expr_str(concat, line);
                        }

                        break;
                case EXPR_V_MINUS:

                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_num(
                                    ((EXPR_NUM *)left)->data -
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform minus operation on "
                                    "variables of non numeric type",
                                    line);
                        }

                        break;
                case EXPR_V_AND:;

                        result = (EXPR_OP *)create_expr_bool(
                            is_truthy(left) && is_truthy(right), line);

                        break;
                        ;
                case EXPR_V_OR:;
                        result = (EXPR_OP *)create_expr_bool(
                            is_truthy(left) || is_truthy(right), line);

                        break;
                case EXPR_V_EQUAL_EQUAL:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                EXPR_NUM *left_num  = (EXPR_NUM *)left;
                                EXPR_NUM *right_num = (EXPR_NUM *)right;

                                return (EXPR_OP *)create_expr_bool(
                                    left_num->data == right_num->data, line);

                        } else if (left->expr_t == EXPR_T_STRING &&
                                   right->expr_t == EXPR_T_STRING) {
                                EXPR_STR *left_str  = (EXPR_STR *)left;
                                EXPR_STR *right_str = (EXPR_STR *)right;

                                return (EXPR_OP *)create_expr_bool(
                                    strcmp(left_str->data, right_str->data) ==
                                        0,
                                    line);
                        } else if (left->expr_t == EXPR_T_BOOL &&
                                   right->expr_t == EXPR_T_BOOL) {
                                return (EXPR_OP *)create_expr_bool(
                                    is_truthy(left) == is_truthy(right), line);
                        } else {
                                return (EXPR_OP *)create_expr_bool(0, line);
                        }
                        break;
                case EXPR_V_MULTIPLY:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_num(
                                    ((EXPR_NUM *)left)->data *
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform multiply operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_DIVIDE:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_num(
                                    ((EXPR_NUM *)left)->data /
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform divide operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_GREATER:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_bool(
                                    ((EXPR_NUM *)left)->data >
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform greater-than operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_LESS:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_bool(
                                    ((EXPR_NUM *)left)->data <
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform less-than operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_GREATER_EQ:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_bool(
                                    ((EXPR_NUM *)left)->data >=
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform greater-than-equal "
                                    "operation on variables of non numeric "
                                    "type",
                                    line);
                        }
                        break;
                case EXPR_V_LESS_EQ:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (EXPR_OP *)create_expr_bool(
                                    ((EXPR_NUM *)left)->data <=
                                        ((EXPR_NUM *)right)->data,
                                    line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform less-than-equal operation "
                                    "on variables of non numeric type",
                                    line);
                        }
                        break;
                default:
                        break;
                }

                /**
                 * Left and right evaluations are have been copied from
                 * copy_leaf(). Free them since we are backtracking up the tree
                 */

                free(left);
                free(right);

                return result;
                break;
        case EXPR_T_UNARY:;  // <-- very important semicolon. do not remove.
                EXPR_UNR_OP *unrop = (EXPR_UNR_OP *)tree;
                EXPR_OP *    body  = evaluate_expr(unrop->body, env);

                switch (unrop->op) {
                case EXPR_V_NOT:

                        result =
                            (EXPR_OP *)create_expr_bool(!is_truthy(body), line);

                        break;
                case EXPR_V_MINUS:

                        if (body->expr_t == EXPR_T_NUMBER) {
                                EXPR_NUM *num = (EXPR_NUM *)body;

                                result = (EXPR_OP *)create_expr_num(
                                    0.0 - num->data, line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot make non-numeric type negative",
                                    line);
                        }

                        break;
                default:
                        fprintf(stderr,
                                "bug: evaluate_expr: unmatched unary operator");
                        break;
                }

                /**
                 * Free the body copied by copy_leaf()
                 */
                free(body);

                return result;
        case EXPR_T_VAR:;

                EXPR_VAR *var = (EXPR_VAR *)tree;

                EXPR_OP *val = get_value(env, var->var);

                return copy_leaf(val);

        default:
                return copy_leaf(tree);
                break;
        }
}
