#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/parse_errors.h"
#include "../types.h"
#include "../utils/utils.h"
#include "expression_utils.h"

EXPR_OP *parse_equality(TOKEN **current);

EXPR_V _to_op(TOKEN *token) {
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
        default:
                return -1;
        }
}

int op_at_end(TOKEN **current) { return (*current) == NULL; }

EXPR_V peek_op(TOKEN **current) {
        if (op_at_end(current)) return -1;

        return _to_op(*current);
}

int match_op(TOKEN **current, EXPR_V op) {
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
EXPR_BIN_OP *parse_left_assoc_bin_op(
    TOKEN **current,  // double pointer to current position in list
    EXPR_OP *(*left)(
        TOKEN **),  // function to parse left side of binary operator
    EXPR_OP *(*right)(
        TOKEN **),  // function to parse right side of binary operator
    EXPR_V ops[],   // list of operators
    int    ops_sz   // length of ops[] list
) {
        EXPR_OP *l = left(current);

        while (1) {
                EXPR_V op = -1;

                for (int i = 0; i < ops_sz; i++) {
                        if (!match_op(current, ops[i])) continue;

                        op = ops[i];
                        break;
                }

                if (op == (EXPR_V)-1) break;

                EXPR_OP *r = right(current);

                EXPR_BIN_OP *root = create_expr_bin_op(op);

                root->left  = l;
                root->right = r;

                l = (EXPR_OP *)root;
        }

        return (EXPR_BIN_OP *)l;
}

EXPR_OP *parse_primary(TOKEN **current) {
        TOKEN *old = *current;

        if (match_op(current, EXPR_V_LEFT_PAREN)) {
                EXPR_OP *primary = (EXPR_OP *)parse_equality(current);

                if (!match_op(current, EXPR_V_RIGHT_PAREN)) {
                        parse_error("Expected closing parentheses.",
                                    (*current)->line);
                }
                return primary;
        }

        if (match_op(current, EXPR_V_STRING)) {
                EXPR_OP *str = (EXPR_OP *)create_expr_str(old->value);

                return str;
        }

        if (match_op(current, EXPR_V_NUMBER)) {
                EXPR_OP *num = (EXPR_OP *)create_expr_num(old->value);

                return num;
        }

        if (match_op(current, EXPR_V_TRUE)) {
                EXPR_OP *bl = (EXPR_OP *)create_expr_bool(1);

                return bl;
        }

        if (match_op(current, EXPR_V_FALSE)) {
                EXPR_OP *bl = (EXPR_OP *)create_expr_bool(0);

                return bl;
        }

        register_error(SYNTAX_ERROR, "Unknown expression", current);
        panic(current);
        
        return NULL;
}

EXPR_OP *parse_unary(TOKEN **current) {
        TOKEN *curr_op = *current;

        if (match_op(current, EXPR_V_NOT) || match_op(current, EXPR_V_MINUS)) {
                EXPR_UNR_OP *op = create_expr_unr_op(peek_op(&curr_op));

                op->body = (EXPR_OP *)parse_unary(current);

                return (EXPR_OP *)op;
        } else {
                return parse_primary(current);
        }
}

EXPR_OP *parse_products(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_MULTIPLY, EXPR_V_DIVIDE};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_unary,
                                                  parse_unary, ops, 2);
}

EXPR_OP *parse_sums(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_PLUS, EXPR_V_MINUS};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_products,
                                                  parse_products, ops, 2);
}

EXPR_OP *parse_and(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_AND};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_sums,
                                                  parse_sums, ops, 1);
}

EXPR_OP *parse_or(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_OR};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_and, parse_and,
                                                  ops, 1);
}

EXPR_OP *parse_comparison(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_GREATER, EXPR_V_LESS, EXPR_V_GREATER_EQ,
                        EXPR_V_LESS_EQ};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_or, parse_or,
                                                  ops, 4);
}

EXPR_OP *parse_equality(TOKEN **current) {
        EXPR_V ops[] = {EXPR_V_EQUAL_EQUAL, EXPR_V_BANG_EQUAL};

        return (EXPR_OP *)parse_left_assoc_bin_op(current, parse_comparison,
                                                  parse_comparison, ops, 2);
}

EXPR_OP *parse_expr(TOKEN **current) { return parse_equality(current); }
