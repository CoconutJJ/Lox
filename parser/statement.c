#include <stdlib.h>

#include "../error/parse_errors.h"
#include "../types.h"
#include "assert.h"
#include "expression.h"
#include "statement_utils.h"

WHILE_STATEMENT *     parse_while(TOKEN **current);
IFELSE_STATEMENT *    parse_conditional(TOKEN **current);
ASSIGNMENT_STATEMENT *parse_assignment(TOKEN **current);

inline int is_at_end(TOKEN **current) { return (*current) == NULL; }

int match_token(TOKEN **current, TOKEN_T t) {
        if (is_at_end(current)) {
                return 0;
        }

        if ((*current)->t == t) {
                *current = (*current)->next;
                return 1;
        }
        return 0;
}

TOKEN_T peek_token(TOKEN **current) {
        if (is_at_end(current)) return -1;

        if ((*current)->next) return (*current)->next->t;

        return -1;
}

STATEMENT *parse_stmt(TOKEN **current) {
        STATEMENT * head = NULL;
        STATEMENT **curr = &head;

        while (!is_at_end(current)) {
                switch ((*current)->t) {
                case IF:
                        *curr = (STATEMENT *)parse_conditional(current);
                        break;
                case WHILE:
                        *curr = (STATEMENT *)parse_while(current);
                        break;
                case IDENTIFIER:
                        if (peek_token(current) == EQUAL) {
                                *curr = (STATEMENT *)parse_assignment(current);
                        }
                        break;
                default:
                        break;
                }

                if ((*curr)) {
                        curr = &((*curr)->next);
                } else {
                        break;
                }
        }

        return head;
}

STATEMENT *parse_statements(TOKEN **current) {
        STATEMENT *code = parse_stmt(current);

        if (!is_at_end(current)) {
                register_error(SYNTAX_ERROR, "unknown symbol", current);
                return NULL;
        }

        return code;
}

ASSIGNMENT_STATEMENT *parse_assignment(TOKEN **current) {
        if (!match_token(current, IDENTIFIER)) return NULL;

        if (!match_token(current, EQUAL)) return NULL;

        EXPR_OP *assign_value = parse_expr(current);

        ASSIGNMENT_STATEMENT *new_assignment = create_assignment_stmt();

        return NULL;
}

WHILE_STATEMENT *parse_while(TOKEN **current) {
        WHILE_STATEMENT *new_while = create_while_stmt();

        assert(match_token(current, WHILE));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after WHILE statement",
                               current);
                panic(current);
                return NULL;
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after WHILE statement",
                               current);
                panic(current);
                return NULL;
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after WHILE statement",
                               current);
                panic(current);
                return NULL;
        }

        STATEMENT *body = parse_stmt(current);

        new_while->body      = body;
        new_while->cond_expr = cond;

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after WHILE statement",
                               current);
                panic(current);
                return NULL;
        }

        return new_while;
}

// FOR_STATEMENT *parse_for(TOKEN **current) {}

IFELSE_STATEMENT *parse_conditional(TOKEN **current) {
        assert(match_token(current, IF));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after IF statement",
                               current);
                panic(current);
                return NULL;
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after IF statement",
                               current);
                panic(current);
                return NULL;
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after IF clause",
                               current);
                panic(current);
                return NULL;
        }

        STATEMENT *if_body = parse_stmt(current);

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after IF clause",
                               current);
                panic(current);
                return NULL;
        }

        STATEMENT *else_body = NULL;

        if (match_token(current, ELSE)) {
                if (!match_token(current, LEFT_BRACE)) {
                        register_error(SYNTAX_ERROR,
                                       "missing { after ELSE statement",
                                       current);

                        panic(current);

                        return NULL;
                }

                else_body = parse_stmt(current);

                if (!match_token(current, RIGHT_BRACE)) {
                        register_error(SYNTAX_ERROR,
                                       "missing } after ELSE statement",
                                       current);

                        panic(current);

                        return NULL;
                }
        }

        IFELSE_STATEMENT *new_ifelse = create_ifelse_stmt();

        new_ifelse->cond_expr   = cond;
        new_ifelse->if_clause   = if_body;
        new_ifelse->else_clause = else_body;

        return new_ifelse;
}