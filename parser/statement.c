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
#include <stdlib.h>

#include "../error/errors.h"
#include "../types.h"
#include "assert.h"
#include "expression.h"
#include "statement_utils.h"

WHILE_STATEMENT *      parse_while(TOKEN **current);
IFELSE_STATEMENT *     parse_conditional(TOKEN **current);
ASSIGNMENT_STATEMENT * parse_assignment(TOKEN **current);
DECLARATION_STATEMENT *parse_declaration(TOKEN **current);
PRINT_STATEMENT *      parse_print(TOKEN **current);

int is_at_end(TOKEN **current) { return (*current) == NULL; }

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

/**
 * Peek at the next token in the list.
 */
TOKEN_T peek_token(TOKEN **current) {
        if (is_at_end(current)) return -1;

        if ((*current)->next) return (*current)->next->t;

        return -1;
}

STATEMENT *parse_stmt(TOKEN **current) {
        STATEMENT * head = NULL;
        STATEMENT **curr = &head;

        while (!is_at_end(current)) {
                /**
                 * Match for the start of the statement
                 */
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
                case VAR:
                        *curr = (STATEMENT *)parse_declaration(current);
                        break;
                case PRINT:
                        *curr = (STATEMENT *)parse_print(current);
                        break;
                default:
                        break;
                }

                if (*curr)
                        curr = &((*curr)->next);
                else
                        break;
        }

        return head;
}

/**
 * Entry function to parse list of statements
 */
STATEMENT *parse_statements(TOKEN **current) {
        STATEMENT *code = parse_stmt(current);

        if (!is_at_end(current)) {
                register_error(SYNTAX_ERROR, "unknown symbol",
                               (*current)->line);
                return NULL;
        }

        return code;
}
/**
 * Parse a print() statement
 */
PRINT_STATEMENT *parse_print(TOKEN **current) {
        if (!match_token(current, PRINT)) return NULL;

        if (!match_token(current, LEFT_PAREN)) return NULL;

        EXPR_OP *print_value = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) return NULL;

        PRINT_STATEMENT *new_print = create_print_stmt();

        new_print->value = print_value;

        return new_print;
}

/**
 * Parse an assignment statement
 */
ASSIGNMENT_STATEMENT *parse_assignment(TOKEN **current) {
        TOKEN *ident = *current;

        if (!match_token(current, IDENTIFIER)) return NULL;

        if (!match_token(current, EQUAL)) return NULL;

        EXPR_OP *assign_value = parse_expr(current);

        ASSIGNMENT_STATEMENT *new_assignment = create_assignment_stmt();

        new_assignment->identifier_name  = ident->value;
        new_assignment->identifier_value = assign_value;
        new_assignment->_statement_.line = ident->line;
        return new_assignment;
}

/**
 * Parse a declaration statement
 */
DECLARATION_STATEMENT *parse_declaration(TOKEN **current) {
        if (!match_token(current, VAR)) return NULL;

        TOKEN *ident = *current;

        if (!match_token(current, IDENTIFIER)) return NULL;

        EXPR_OP *assign_value = NULL;

        if (match_token(current, EQUAL)) {
                assign_value = parse_expr(current);
        }

        DECLARATION_STATEMENT *new_declaration = create_declaration_stmt();

        new_declaration->identifier_name  = ident->value;
        new_declaration->identifier_value = assign_value;
        new_declaration->_statement_.line = ident->line;
        return new_declaration;
}

/**
 * Parse a while statement
 */
WHILE_STATEMENT *parse_while(TOKEN **current) {
        WHILE_STATEMENT *new_while = create_while_stmt();

        new_while->_statement_.line = (*current)->line;

        assert(match_token(current, WHILE));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after WHILE statement",
                               (*current)->line);
                return NULL;
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after WHILE statement",
                               (*current)->line);
                return NULL;
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after WHILE statement",
                               (*current)->line);
                return NULL;
        }

        STATEMENT *body = parse_stmt(current);

        new_while->body      = body;
        new_while->cond_expr = cond;

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after WHILE statement",
                               (*current)->line);
                return NULL;
        }

        return new_while;
}

// FOR_STATEMENT *parse_for(TOKEN **current) {}

IFELSE_STATEMENT *parse_conditional(TOKEN **current) {
        IFELSE_STATEMENT *new_ifelse = create_ifelse_stmt();

        new_ifelse->_statment_.line = (*current)->line;

        assert(match_token(current, IF));

        if (!match_token(current, LEFT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ( after IF statement",
                               (*current)->line);
        }

        EXPR_OP *cond = parse_expr(current);

        if (!match_token(current, RIGHT_PAREN)) {
                register_error(SYNTAX_ERROR, "missing ) after IF statement",
                               (*current)->line);
        }

        if (!match_token(current, LEFT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing { after IF clause",
                               (*current)->line);
        }

        STATEMENT *if_body = parse_stmt(current);

        if (!match_token(current, RIGHT_BRACE)) {
                register_error(SYNTAX_ERROR, "missing } after IF clause",
                               (*current)->line);
        }

        STATEMENT *       else_body = NULL;
        IFELSE_STATEMENT *curr      = NULL;

        if (match_token(current, ELSE)) {
                /**
                 * TODO: If-Else-If has bad scoping issues
                 *
                 * The way we chain If-Else-If statements causes issues in the
                 * hashmap scoping method we use during evaluation. The runtime
                 * calls down_scope() everytime an elif condition is evaluated
                 * to be false. This is because the runtime is not aware of
                 * else-if statements and believes we are running a simple else
                 * block.
                 */

                while (1) {
                        if (!match_token(current, IF)) break;

                        if (!match_token(current, LEFT_PAREN)) {
                                register_error(
                                    SYNTAX_ERROR,
                                    "missing ( after ELSE-IF statement",
                                    (*current)->line);
                        }

                        EXPR_OP *elif_cond = parse_expr(current);

                        if (!match_token(current, RIGHT_PAREN)) {
                                register_error(
                                    SYNTAX_ERROR,
                                    "missing ) after ELSE-IF statement",
                                    (*current)->line);
                        }

                        if (!match_token(current, LEFT_BRACE)) {
                                register_error(SYNTAX_ERROR,
                                               "missing { after ELSE-IF clause",
                                               (*current)->line);
                        }

                        IFELSE_STATEMENT *elif = create_ifelse_stmt();

                        elif->cond_expr = elif_cond;
                        elif->if_clause = parse_stmt(current);

                        if (!match_token(current, RIGHT_BRACE)) {
                                register_error(SYNTAX_ERROR,
                                               "missing } after ELSE-IF clause",
                                               (*current)->line);
                        }

                        if (curr) {
                                curr->else_clause = elif;
                        } else {
                                else_body = elif;
                        }

                        /**
                         * Set curr to the next if-else statement in the chain.
                         */

                        curr = elif;

                        /**
                         * Try matching the next ELSE or ELSE-IF clause.
                         *
                         * If it's a standalone ELSE clause, then next iteration
                         * will break since the expected next symbol will be
                         * LEFT_BRACE and not IF.
                         *
                         * If it's a IF clause, then next iteration will proceed
                         * like normal.
                         */
                        if (match_token(current, ELSE)) {
                                continue;
                        } else {
                                /**
                                 * This IF-ELIF-ELSE chain has no ELSE
                                 * statement, skip parsing for LEFT_BRACE and
                                 * RIGHT_BRACE after loop exit, jump directly to
                                 * the return statement assignments.
                                 */
                                goto create_ifelse_body;
                        }
                }

                if (!match_token(current, LEFT_BRACE)) {
                        register_error(SYNTAX_ERROR,
                                       "missing { after ELSE statement",
                                       (*current)->line);

                        return NULL;
                }

                /**
                 * We need to check here if the else clause is standalone
                 * or a footer to a series of else-if clauses.
                 */
                if (curr) {
                        curr->else_clause = parse_stmt(current);
                } else {
                        else_body = parse_stmt(current);
                }

                if (!match_token(current, RIGHT_BRACE)) {
                        register_error(SYNTAX_ERROR,
                                       "missing } after ELSE statement",
                                       (*current)->line);

                        return NULL;
                }
        }

create_ifelse_body:
        new_ifelse->cond_expr   = cond;
        new_ifelse->if_clause   = if_body;
        new_ifelse->else_clause = else_body;

        return new_ifelse;
}