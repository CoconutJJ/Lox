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

#include "../error/errors.h"
#include "../types.h"
#include "../utils/utils.h"
#include "expression_utils.h"

struct expr_op *parse_equality(struct token **current);

enum op_v _to_op(struct token *token) {
        switch (token->t) {
        case AND:
                return EXPR_V_AND;
        case OR:
                return EXPR_V_OR;
        case PLUS:
                return EXPR_V_PLUS;
        case MINUS:
                return EXPR_V_MINUS;
        case SLASH:
                return EXPR_V_DIVIDE;
        case MULTIPLY:
                return EXPR_V_MULTIPLY;
        case BANG:
                return EXPR_V_NOT;
        case GREATER:
                return EXPR_V_GREATER;
        case LESS:
                return EXPR_V_LESS;
        case GREATER_EQ:
                return EXPR_V_GREATER_EQ;
        case LESS_EQ:
                return EXPR_V_LESS_EQ;
        case EQUAL_EQUAL:
                return EXPR_V_EQUAL_EQUAL;
        case BANG_EQUAL:
                return EXPR_V_BANG_EQUAL;
        case LEFT_PAREN:
                return EXPR_V_LEFT_PAREN;
        case RIGHT_PAREN:
                return EXPR_V_RIGHT_PAREN;
        case STRING:
                return EXPR_V_STRING;
        case NUMBER:
                return EXPR_V_NUMBER;
        case TRUE:
                return EXPR_V_TRUE;
        case FALSE:
                return EXPR_V_FALSE;
        case IDENTIFIER:
                return EXPR_V_VAR;
        default:
                return -1;
        }
}

int op_at_end(struct token **current) { return (*current) == NULL; }

enum op_v peek_op(struct token **current) {
        if (op_at_end(current)) return -1;

        return _to_op(*current);
}

int match_op(struct token **current, enum op_v op) {
        if (peek_op(current) == op) {
                *current = (*current)->next;
                return 1;
        }
        return 0;
}

/**
 *  Parses a left-associative binary operator, where *left is run on the l.h.s
 *  of the operator, and *right is run on the r.h.s of the operator
 */
struct expr_bin_op *parse_left_assoc_bin_op(
    struct token **current,  // double pointer to current position in list
    struct expr_op *(*left)(
        struct token **),  // function to parse left side of binary operator
    struct expr_op *(*right)(
        struct token **),  // function to parse right side of binary operator
    enum op_v ops[],       // list of operators
    int ops_sz             // length of ops[] list
) {
        struct expr_op *l = left(current);

        while (1) {
                enum op_v op = -1;

                for (int i = 0; i < ops_sz; i++) {
                        if (!match_op(current, ops[i])) continue;

                        op = ops[i];
                        break;
                }

                if (op == (enum op_v) - 1) break;

                int line = (*current)->line;

                struct expr_op *r = right(current);

                struct expr_bin_op *root = create_expr_bin_op(op, line);

                root->left = l;
                root->right = r;

                l = (struct expr_op *)root;
        }

        return (struct expr_bin_op *)l;
}

struct expr_op *parse_primary(struct token **current) {
        struct token *old = *current;
        int line = old->line;
        if (match_op(current, EXPR_V_LEFT_PAREN)) {
                struct expr_op *primary =
                    (struct expr_op *)parse_equality(current);

                if (!match_op(current, EXPR_V_RIGHT_PAREN)) {
                        register_error(PARSE_ERROR,
                                       "Expected closing parentheses.",
                                       (*current)->line);
                }
                return primary;
        }

        if (match_op(current, EXPR_V_STRING)) {
                struct expr_op *str =
                    (struct expr_op *)create_expr_str(old->value, line);

                return str;
        }

        if (match_op(current, EXPR_V_NUMBER)) {
                struct expr_op *num = (struct expr_op *)create_expr_num(
                    strtod(old->value, NULL), line);

                return num;
        }

        if (match_op(current, EXPR_V_TRUE)) {
                struct expr_op *bl =
                    (struct expr_op *)create_expr_bool(1, line);

                return bl;
        }

        if (match_op(current, EXPR_V_FALSE)) {
                struct expr_op *bl =
                    (struct expr_op *)create_expr_bool(0, line);

                return bl;
        }

        if (match_op(current, EXPR_V_VAR)) {
                struct expr_op *var =
                    (struct expr_op *)create_expr_var(old->value, line);

                return var;
        }

        register_error(SYNTAX_ERROR, "Unknown expression", (*current)->line);

        return NULL;
}

struct expr_op *parse_unary(struct token **current) {
        struct token *curr_op = *current;
        int line = curr_op->line;
        if (match_op(current, EXPR_V_NOT) || match_op(current, EXPR_V_MINUS)) {
                struct expr_unr_op *op =
                    create_expr_unr_op(peek_op(&curr_op), line);

                op->body = (struct expr_op *)parse_unary(current);

                return (struct expr_op *)op;
        } else {
                return parse_primary(current);
        }
}

struct expr_op *parse_products(struct token **current) {
        enum op_v ops[] = {EXPR_V_MULTIPLY, EXPR_V_DIVIDE};

        return (struct expr_op *)parse_left_assoc_bin_op(current, parse_unary,
                                                         parse_unary, ops, 2);
}

struct expr_op *parse_sums(struct token **current) {
        enum op_v ops[] = {EXPR_V_PLUS, EXPR_V_MINUS};

        return (struct expr_op *)parse_left_assoc_bin_op(
            current, parse_products, parse_products, ops, 2);
}

struct expr_op *parse_and(struct token **current) {
        enum op_v ops[] = {EXPR_V_AND};

        return (struct expr_op *)parse_left_assoc_bin_op(current, parse_sums,
                                                         parse_sums, ops, 1);
}

struct expr_op *parse_or(struct token **current) {
        enum op_v ops[] = {EXPR_V_OR};

        return (struct expr_op *)parse_left_assoc_bin_op(current, parse_and,
                                                         parse_and, ops, 1);
}

struct expr_op *parse_comparison(struct token **current) {
        enum op_v ops[] = {EXPR_V_GREATER, EXPR_V_LESS, EXPR_V_GREATER_EQ,
                           EXPR_V_LESS_EQ};

        return (struct expr_op *)parse_left_assoc_bin_op(current, parse_or,
                                                         parse_or, ops, 4);
}

struct expr_op *parse_equality(struct token **current) {
        enum op_v ops[] = {EXPR_V_EQUAL_EQUAL, EXPR_V_BANG_EQUAL};

        return (struct expr_op *)parse_left_assoc_bin_op(
            current, parse_comparison, parse_comparison, ops, 2);
}

struct expr_op *parse_expr(struct token **current) {
        return parse_equality(current);
}