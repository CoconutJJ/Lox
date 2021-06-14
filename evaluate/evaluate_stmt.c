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

#include "../environment/environment.h"
#include "../error/errors.h"
#include "../types.h"
#include "../utils/utils.h"
#include "./evaluate_expr.h"

void evaluate_statements(struct statement_t* stmt, struct environment* env);

void evaluate_ifelse_statement(struct ifelse_statement* stmt,
                               struct environment* env) {
        struct expr_op* res = evaluate_expr(stmt->cond_expr, env);
        if (is_truthy(res)) {
                free(res);
                env = down_scope(env);

                evaluate_statements(stmt->if_clause, env);

                env = up_scope(env);
        } else {
                free(res);
                if (!stmt->else_clause) return;

                env = down_scope(env);

                evaluate_statements(stmt->else_clause, env);

                env = up_scope(env);
        }
}

void evaluate_while_statement(struct while_statement* stmt,
                              struct environment* env) {
        struct expr_op* res;
        while (is_truthy(res = evaluate_expr(stmt->cond_expr, env))) {
                env = down_scope(env);

                evaluate_statements(stmt->body, env);

                env = up_scope(env);
                free(res);
        }
        free(res);
}

void evaluate_assignment_statement(struct assignment_statement* stmt,
                                   struct environment* env) {
        struct expr_op* res = evaluate_expr(stmt->identifier_value, env);
        int data_size;

        switch (res->expr_t) {
        case EXPR_T_NUMBER:
                data_size = sizeof(struct expr_number);
                break;
        case EXPR_T_STRING:
                data_size = sizeof(struct expr_str);
                break;
        case EXPR_T_BOOL:
                data_size = sizeof(struct expr_bool);
                break;
        default:
                UNREACHABLE("bug: assignment value evaluated to non-leaf node");
                break;
        }

        if (!update_value(env, stmt->identifier_name, res, data_size)) {
                register_error(RUNTIME_ERROR,
                               "assignment to undeclared variable",
                               stmt->_statement_.line);
        }
        free(res);
}

void evaluate_declaration_statement(struct declaration_statement* stmt,
                                    struct environment* env) {
        struct expr_op* res;
        int data_size;

        if (stmt->identifier_value) {
                res = evaluate_expr(stmt->identifier_value, env);

                switch (res->expr_t) {
                case EXPR_T_NUMBER:
                        data_size = sizeof(struct expr_number);
                        break;
                case EXPR_T_STRING:
                        data_size = sizeof(struct expr_str);
                        break;
                case EXPR_T_BOOL:
                        data_size = sizeof(struct expr_bool);
                        break;
                default:
                        break;
                }
        } else {
                res = NULL;
                data_size = 0;
        }

        set_value(env, stmt->identifier_name, res, data_size);
        free(res);
}

void evaluate_print_statement(struct print_statement* stmt,
                              struct environment* env) {
        struct expr_op* res = evaluate_expr(stmt->value, env);

        switch (res->expr_t) {
        case EXPR_T_NUMBER:;
                struct expr_number* num = (struct expr_number*)res;
                printf("%lf", num->data);
                break;
        case EXPR_T_STRING:;
                struct expr_str* str = (struct expr_str*)res;
                printf("%s", str->data);
                break;
        case EXPR_T_BOOL:;
                struct expr_bool* bl = (struct expr_bool*)res;

                if (bl->data) {
                        printf("TRUE");
                } else {
                        printf("FALSE");
                }
                break;

        default:
                break;
        }
        free(res);
}

void evaluate_statements(struct statement_t* stmt, struct environment* env) {
        while (stmt) {
                switch (stmt->type) {
                case E_IFELSE_STATEMENT:
                        evaluate_ifelse_statement(
                            (struct ifelse_statement*)stmt, env);
                        break;
                case E_WHILE_STATEMENT:
                        evaluate_while_statement((struct while_statement*)stmt,
                                                 env);
                        break;
                case E_ASSIGNMENT_STATEMENT:
                        evaluate_assignment_statement(
                            (struct assignment_statement*)stmt, env);
                        break;
                case E_DECLARATION_STATEMENT:
                        evaluate_declaration_statement(
                            (struct declaration_statement*)stmt, env);
                        break;
                case E_PRINT_STATEMENT:
                        evaluate_print_statement((struct print_statement*)stmt,
                                                 env);
                        break;
                default:
                        break;
                }

                stmt = stmt->next;
        }
}
