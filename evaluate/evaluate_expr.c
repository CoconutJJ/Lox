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
#include "../utils/utils.h"

/**
 * Evaluates a Number, String or Bool unit expression and decides whether the
 * value is a truthy value or falsey value.
 */
int is_truthy(struct expr_op *val) {
        switch (val->expr_t) {
        case EXPR_T_NUMBER:;
                struct expr_number *num = (struct expr_number *)val;

                return num->data != 0.0;
                break;
        case EXPR_T_STRING:;
                struct expr_str *str = (struct expr_str *)val;

                return strlen(str->data) != 0;
                break;
        case EXPR_T_BOOL:;

                struct expr_bool *bl = (struct expr_bool *)val;

                return bl->data;
                break;
        default:
                UNREACHABLE("bug: is_truthy: recieved invalid expression");
                break;
        }
}

/**
 * Deep copies a leaf node from the expression tree and returns the new EXPR
 * struct.
 */
struct expr_op *copy_leaf(struct expr_op *op) {
        switch (op->expr_t) {
        case EXPR_T_NUMBER:;
                struct expr_number *num = (struct expr_number *)op;
                return (struct expr_op *)create_expr_num(num->data, op->line);
                break;
        case EXPR_T_STRING:;
                struct expr_str *str = (struct expr_str *)op;
                return (struct expr_op *)create_expr_str(str->data, op->line);
                break;
        case EXPR_T_BOOL:;
                struct expr_bool *bl = (struct expr_bool *)op;
                return (struct expr_op *)create_expr_bool(bl->data, op->line);
                break;
        case EXPR_T_VAR:;
                struct expr_var *var = (struct expr_var *)op;
                return (struct expr_op *)create_expr_var(var->var, op->line);
                break;
        default:
                UNREACHABLE("bug: copy_leaf: recieved invalid enum");
                break;
        }
}

struct expr_op *evaluate_plus(struct expr_op *left, struct expr_op *right,
                              int line) {
        switch (left->expr_t | right->expr_t) {
        case EXPR_T_NUMBER:
                return (struct expr_op *)create_expr_num(
                    ((struct expr_number *)left)->data +
                        ((struct expr_number *)right)->data,
                    line);
                break;
        case EXPR_T_STRING:;
                struct expr_str *left_str = (struct expr_str *)left;
                struct expr_str *right_str = (struct expr_str *)right;

                char *concat = malloc(strlen(left_str->data) +
                                      strlen(right_str->data) + 1);

                strcpy(concat, left_str->data);
                strcat(concat, right_str->data);

                struct expr_op *ret =
                    (struct expr_op *)create_expr_str(concat, line);
                free(concat);
                return ret;
                break;
        default:
                return NULL;
        }
}

struct expr_op *evaluate_minus(struct expr_op *left, struct expr_op *right,
                               int line) {
        if (left->expr_t == EXPR_T_NUMBER && right->expr_t == EXPR_T_NUMBER) {
                return (struct expr_op *)create_expr_num(
                    ((struct expr_number *)left)->data +
                        ((struct expr_number *)right)->data,
                    line);

        } else {
                return NULL;
        }
}

struct expr_op *evaluate_and(struct expr_op *left, struct expr_op *right,
                             int line) {
        return (struct expr_op *)create_expr_bool(
            is_truthy(left) && is_truthy(right), line);
}

struct expr_op *evaluate_or(struct expr_op *left, struct expr_op *right,
                            int line) {
        return (struct expr_op *)create_expr_bool(
            is_truthy(left) || is_truthy(right), line);
}

struct expr_op *evaluate_equal_equal(struct expr_op *left,
                                     struct expr_op *right, int line) {
        switch (left->expr_t | right->expr_t) {
        case EXPR_T_NUMBER:

                return create_expr_bool(
                    CAST_PROP(struct expr_number *, left, data) ==
                        CAST_PROP(struct expr_number *, right, data),
                    line);

                break;
        case EXPR_T_STRING:

                return create_expr_bool(
                    strcmp(CAST_PROP(struct expr_str *, left, data),
                           CAST_PROP(struct expr_str *, right, data)),
                    line);

                break;
        case EXPR_T_BOOL:
                break;
        default:
                break;
        }
}

struct expr_op *evaluate_expr(struct expr_op *tree, struct environment *env) {
        if (!tree) return NULL;

        struct expr_op *result;
        int line = tree->line;
        switch (tree->expr_t) {
        case EXPR_T_BINARY:;  // <-- very important semicolon, do not remove.

                struct expr_bin_op *binop = (struct expr_bin_op *)tree;

                struct expr_op *left = evaluate_expr(binop->left, env);
                struct expr_op *right = evaluate_expr(binop->right, env);

                switch (binop->op) {
                case EXPR_V_PLUS:
                        result = evaluate_plus(left, right, line);
                        if (!result) {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform plus operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_MINUS:
                        result = evaluate_minus(left, right, line);
                        if (!result) {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot perform minus operation on "
                                    "variables of non numeric type",
                                    line);
                        }
                        break;
                case EXPR_V_AND:;
                        result = evaluate_and(left, right, line);
                        break;
                case EXPR_V_OR:;
                        result = evaluate_or(left, right, line);
                        break;
                case EXPR_V_EQUAL_EQUAL:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                struct expr_number *left_num =
                                    (struct expr_number *)left;
                                struct expr_number *right_num =
                                    (struct expr_number *)right;

                                return (struct expr_op *)create_expr_bool(
                                    left_num->data == right_num->data, line);

                        } else if (left->expr_t == EXPR_T_STRING &&
                                   right->expr_t == EXPR_T_STRING) {
                                struct expr_str *left_str =
                                    (struct expr_str *)left;
                                struct expr_str *right_str =
                                    (struct expr_str *)right;

                                return (struct expr_op *)create_expr_bool(
                                    strcmp(left_str->data, right_str->data) ==
                                        0,
                                    line);
                        } else if (left->expr_t == EXPR_T_BOOL &&
                                   right->expr_t == EXPR_T_BOOL) {
                                return (struct expr_op *)create_expr_bool(
                                    is_truthy(left) == is_truthy(right), line);
                        } else {
                                return (struct expr_op *)create_expr_bool(0,
                                                                          line);
                        }
                        break;
                case EXPR_V_BANG_EQUAL:
                        if (left->expr_t != right->expr_t) {
                                return (struct expr_op *)create_expr_bool(1,
                                                                          line);
                        }

                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                struct expr_number *left_num =
                                    (struct expr_number *)left;
                                struct expr_number *right_num =
                                    (struct expr_number *)right;

                                return (struct expr_op *)create_expr_bool(
                                    left_num->data != right_num->data, line);

                        } else if (left->expr_t == EXPR_T_STRING &&
                                   right->expr_t == EXPR_T_STRING) {
                                struct expr_str *left_str =
                                    (struct expr_str *)left;
                                struct expr_str *right_str =
                                    (struct expr_str *)right;

                                return (struct expr_op *)create_expr_bool(
                                    strcmp(left_str->data, right_str->data) !=
                                        0,
                                    line);
                        } else if (left->expr_t == EXPR_T_BOOL &&
                                   right->expr_t == EXPR_T_BOOL) {
                                return (struct expr_op *)create_expr_bool(
                                    is_truthy(left) != is_truthy(right), line);
                        }

                        break;
                case EXPR_V_MULTIPLY:
                        if (left->expr_t == EXPR_T_NUMBER &&
                            right->expr_t == EXPR_T_NUMBER) {
                                result = (struct expr_op *)create_expr_num(
                                    ((struct expr_number *)left)->data *
                                        ((struct expr_number *)right)->data,
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
                                result = (struct expr_op *)create_expr_num(
                                    ((struct expr_number *)left)->data /
                                        ((struct expr_number *)right)->data,
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
                                result = (struct expr_op *)create_expr_bool(
                                    ((struct expr_number *)left)->data >
                                        ((struct expr_number *)right)->data,
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
                                result = (struct expr_op *)create_expr_bool(
                                    ((struct expr_number *)left)->data <
                                        ((struct expr_number *)right)->data,
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
                                result = (struct expr_op *)create_expr_bool(
                                    ((struct expr_number *)left)->data >=
                                        ((struct expr_number *)right)->data,
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
                                result = (struct expr_op *)create_expr_bool(
                                    ((struct expr_number *)left)->data <=
                                        ((struct expr_number *)right)->data,
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
                struct expr_unr_op *unrop = (struct expr_unr_op *)tree;
                struct expr_op *body = evaluate_expr(unrop->body, env);

                switch (unrop->op) {
                case EXPR_V_NOT:

                        result = (struct expr_op *)create_expr_bool(
                            !is_truthy(body), line);

                        break;
                case EXPR_V_MINUS:

                        if (body->expr_t == EXPR_T_NUMBER) {
                                struct expr_number *num =
                                    (struct expr_number *)body;

                                result = (struct expr_op *)create_expr_num(
                                    0.0 - num->data, line);
                        } else {
                                register_error(
                                    RUNTIME_ERROR,
                                    "Cannot make non-numeric type negative",
                                    line);
                        }

                        break;
                default:
                        UNREACHABLE(
                            "bug: evaluate_expr: unmatched unary operator");
                        break;
                }

                /**
                 * Free the body copied by copy_leaf()
                 */
                free(body);

                return result;
        case EXPR_T_VAR:;

                struct expr_var *var = (struct expr_var *)tree;

                struct expr_op *val = get_value(env, var->var);

                return copy_leaf(val);

        default:
                return copy_leaf(tree);
                break;
        }
}
