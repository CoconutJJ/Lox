#include "./evaluate_stmt.h"
#include "../environment/environment.h"
#include "../types.h"

void evaluate_ifelse_statement(IFELSE_STATEMENT* stmt, ENVIRONMENT* env) {
        if (is_truthy(evaluate_expr(stmt->cond_expr, env))) {
                env = down_scope(env);

                evaluate_statements(stmt->if_clause, env);

                env = up_scope(env);

        } else {
                if (!stmt->else_clause) return;

                env = down_scope(env);

                evaluate_statements(stmt->else_clause, env);

                env = up_scope(env);
        }
}

void evaluate_while_statement(WHILE_STATEMENT* stmt, ENVIRONMENT* env) {
        
        while (is_truthy(evaluate_expr(stmt->cond_expr, env))) {
                env = down_scope(env);

                evaluate_statements(stmt->body, env);

                env = up_scope(env);
        }
}

void evaluate_assignment_statement(ASSIGNMENT_STATEMENT* stmt,
                                   ENVIRONMENT*          env) {
        EXPR_OP* res = evaluate_expr(stmt->identifier_value, env);
        int      data_size;

        switch (res->expr_t) {
        case EXPR_V_NUMBER:
                data_size = sizeof(EXPR_NUM);
                break;
        case EXPR_V_STRING:
                data_size = sizeof(EXPR_STR);
                break;
        case EXPR_V_FALSE:
        case EXPR_V_TRUE:
                data_size = sizeof(EXPR_BOOL);
                break;
        }

        update_value(env, stmt->identifier_name, res, data_size);
}

void evaluate_declaration_statement(DECLARATION_STATEMENT* stmt,
                                    ENVIRONMENT*           env) {
        EXPR_OP* res = evaluate_expr(stmt->identifier_value, env);
        int      data_size;

        switch (res->expr_t) {
        case EXPR_V_NUMBER:
                data_size = sizeof(EXPR_NUM);
                break;
        case EXPR_V_STRING:
                data_size = sizeof(EXPR_STR);
                break;
        case EXPR_V_FALSE:
        case EXPR_V_TRUE:
                data_size = sizeof(EXPR_BOOL);
                break;
        }

        set_value(env, stmt->identifier_name, res, data_size);
}

void evaluate_for_statement(FOR_STATEMENT* stmt, ENVIRONMENT* env) {}

void evaluate_statements(STATEMENT* stmt, ENVIRONMENT* env) {

        while (stmt) {
                switch (stmt->type) {
                case E_IFELSE_STATEMENT:
                        evaluate_ifelse_statement(stmt, env);
                        break;
                case E_WHILE_STATEMENT:
                        evaluate_while_statement(stmt, env);
                        break;
                case E_ASSIGNMENT_STATEMENT:
                        evaluate_assignment_statement(stmt, env);
                        break;
                case E_DECLARATION_STATEMENT:
                        evaluate_declaration_statement(stmt, env);
                        break;
                default:
                        break;
                }

                stmt = stmt->next;
        }

}
