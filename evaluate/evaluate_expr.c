#include <stdio.h>
#include <stdlib.h>

#include "../parser/expression.h"
#include "../types.h"
int isTruthy(EXPR_OP *val) {}


EXPR_OP *evaluate_expr(EXPR_OP *tree) {
        if (!tree) return NULL;

        switch (tree->expr_t) {
        case EXPR_T_BINARY:;  // <-- very important semicolon, do not remove.

                EXPR_BIN_OP *binop = (EXPR_BIN_OP *)tree;

                EXPR_OP *left  = evaluate_expr(binop->left);
                EXPR_OP *right = evaluate_expr(binop->right);

                switch (binop->op) {
                case EXPR_V_PLUS:

                        left  = evaluate_expr(left);
                        right = evaluate_expr(right);

                        if (left->expr_t == EXPR_V_NUMBER &&
                            right->expr_t == EXPR_V_NUMBER) {
                                ((EXPR_NUM *)left)->data +
                                    ((EXPR_NUM *)right)->data;
                        }

                        break;
                case EXPR_V_MINUS:
                        break;
                case EXPR_V_AND:
                        break;
                case EXPR_V_OR:
                        break;
                case EXPR_V_EQUAL_EQUAL:
                        break;
                case EXPR_V_MULTIPLY:
                        break;
                case EXPR_V_DIVIDE:
                        break;
                case EXPR_V_GREATER:
                        break;
                case EXPR_V_LESS:
                        break;
                case EXPR_V_GREATER_EQ:
                        break;
                case EXPR_V_LESS_EQ:
                        break;
                default:
                        break;
                }

                break;
        case EXPR_T_UNARY:;  // <-- very important semicolon. do not remove.
                EXPR_UNR_OP *unrop = (EXPR_UNR_OP *)tree;
 
                switch (unrop->op) {
                case EXPR_V_NOT:
                        /* code */
                        break;
                case EXPR_V_MINUS:

                        break;
                default:
                        break;
                }
        default:
                break;
        }
}
