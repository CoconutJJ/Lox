#include "../error/parse_errors.h"
#include "../types.h"
#include "assert.h"
#include "expression.h"
#include <stdlib.h>

WHILE_STATEMENT *parse_while(TOKEN **current);
IFELSE_STATEMENT *parse_conditional(TOKEN **current);
ASSIGNMENT_STATEMENT *parse_assignment(TOKEN **current);

int is_at_end(TOKEN **current) { return (*current) == NULL; }

int match_token(TOKEN **current, TOKEN_T t) {

        if (is_at_end(current)) {
                register_error(SYNTAX_ERROR, "Unexpected end of input", 0);
                return 0;
        }

        if ((*current)->t == t) {
                *current = (*current)->next;
                return 1;
        }
        return 0;
}

STATEMENT *parse_statements(TOKEN **current) {

        STATEMENT *head = NULL;
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
                        if ((*current)->next->t == EQUAL) {
                                *curr = (STATEMENT *)parse_assignment(current);
                        }
                        break;
                default:
                        break;
                }

                if ((*curr)) {
                        curr = &((*curr)->next);
                }
        }

        return head;
}

ASSIGNMENT_STATEMENT *parse_assignment(TOKEN **current) {

        if (!match_token(current, IDENTIFIER))
                return NULL;

        if (!match_token(current, EQUAL))
                return NULL;
}

WHILE_STATEMENT *parse_while(TOKEN **current) {

        WHILE_STATEMENT *new_while = malloc(sizeof(WHILE_STATEMENT));

        assert(match_token(current, WHILE));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after WHILE statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after WHILE statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after WHILE statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        STATEMENT *body = parse_statements(current);

        new_while->_statement_.type = E_WHILE_STATEMENT;
        new_while->body = body;
        new_while->cond_expr = cond;

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after WHILE statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        return new_while;
}

FOR_STATEMENT *parse_for(TOKEN **current) {}

IFELSE_STATEMENT *parse_conditional(TOKEN **current) {
        assert(match_token(current, IF));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after IF statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after IF statement",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after IF clause",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        STATEMENT *if_body = parse_statements(current);

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after IF clause",
                               (*current)->line);
                panic(current);
                return NULL;
        }

        STATEMENT *else_body = NULL;

        if (match_token(current, ELSE)) {

                if (!match_token(current, LEFT_BRACE)) {

                        register_error(SYNTAX_ERROR,
                                       "missing { after ELSE statement",
                                       (*current)->line);

                        panic(current);

                        return NULL;
                }

                else_body = parse_statements(current);

                if (!match_token(current, RIGHT_BRACE)) {

                        register_error(SYNTAX_ERROR,
                                       "missing } after ELSE statement",
                                       (*current)->line);

                        panic(current);

                        return NULL;
                }
        }

        IFELSE_STATEMENT *new_ifelse = malloc(sizeof(IFELSE_STATEMENT));

        new_ifelse->_statment_.type = E_IFELSE_STATEMENT;
        new_ifelse->cond_expr = cond;
        new_ifelse->if_clause = if_body;
        new_ifelse->else_clause = else_body;

        return new_ifelse;
}