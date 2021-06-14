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

#include "../types.h"

struct while_statement* create_while_stmt() {
        struct while_statement* new_while =
            malloc(sizeof(struct while_statement));

        if (!new_while) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_while->_statement_.next = NULL;
        new_while->_statement_.type = E_WHILE_STATEMENT;
        new_while->body = NULL;
        new_while->cond_expr = NULL;

        return new_while;
}

struct ifelse_statement* create_ifelse_stmt() {
        struct ifelse_statement* new_ifelse =
            malloc(sizeof(struct ifelse_statement));

        if (!new_ifelse) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_ifelse->_statment_.next = NULL;
        new_ifelse->_statment_.type = E_IFELSE_STATEMENT;
        new_ifelse->cond_expr = NULL;
        new_ifelse->else_clause = NULL;
        new_ifelse->if_clause = NULL;

        return new_ifelse;
}

struct assignment_statement* create_assignment_stmt() {
        struct assignment_statement* new_assignment =
            malloc(sizeof(struct assignment_statement));

        if (!new_assignment) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_assignment->_statement_.next = NULL;
        new_assignment->_statement_.type = E_ASSIGNMENT_STATEMENT;
        new_assignment->identifier_name = NULL;
        new_assignment->identifier_value = NULL;

        return new_assignment;
}

struct declaration_statement* create_declaration_stmt() {
        struct declaration_statement* new_declaration =
            malloc(sizeof(struct declaration_statement));

        if (!new_declaration) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_declaration->_statement_.next = NULL;
        new_declaration->_statement_.type = E_DECLARATION_STATEMENT;
        new_declaration->identifier_name = NULL;
        new_declaration->identifier_value = NULL;

        return new_declaration;
}

struct print_statement* create_print_stmt() {
        struct print_statement* new_print =
            malloc(sizeof(struct print_statement));

        if (!new_print) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_print->_statement_.next = NULL;
        new_print->_statement_.type = E_PRINT_STATEMENT;
        new_print->value = NULL;

        return new_print;
}