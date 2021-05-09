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

WHILE_STATEMENT* create_while_stmt() {
        WHILE_STATEMENT* new_while = malloc(sizeof(WHILE_STATEMENT));

        if (!new_while) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_while->_statement_.next = NULL;
        new_while->_statement_.type = E_WHILE_STATEMENT;
        new_while->body             = NULL;
        new_while->cond_expr        = NULL;

        return new_while;
}

IFELSE_STATEMENT* create_ifelse_stmt() {
        IFELSE_STATEMENT* new_ifelse = malloc(sizeof(IFELSE_STATEMENT));

        if (!new_ifelse) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_ifelse->_statment_.next = NULL;
        new_ifelse->_statment_.type = E_IFELSE_STATEMENT;
        new_ifelse->cond_expr       = NULL;
        new_ifelse->else_clause     = NULL;
        new_ifelse->if_clause       = NULL;

        return new_ifelse;
}

ASSIGNMENT_STATEMENT* create_assignment_stmt() {
        ASSIGNMENT_STATEMENT* new_assignment =
            malloc(sizeof(ASSIGNMENT_STATEMENT));

        if (!new_assignment) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_assignment->_statement_.next = NULL;
        new_assignment->_statement_.type = E_ASSIGNMENT_STATEMENT;
        new_assignment->identifier_name  = NULL;
        new_assignment->identifier_value = NULL;

        return new_assignment;
}

DECLARATION_STATEMENT* create_declaration_stmt() {
        DECLARATION_STATEMENT* new_declaration =
            malloc(sizeof(DECLARATION_STATEMENT));

        if (!new_declaration) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_declaration->_statement_.next = NULL;
        new_declaration->_statement_.type = E_DECLARATION_STATEMENT;
        new_declaration->identifier_name  = NULL;
        new_declaration->identifier_value = NULL;

        return new_declaration;
}

PRINT_STATEMENT * create_print_stmt() {

        PRINT_STATEMENT * new_print = malloc(sizeof(PRINT_STATEMENT));

        if (!new_print) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_print->_statement_.next = NULL;
        new_print->_statement_.type = E_PRINT_STATEMENT;
        new_print->value = NULL;
        
        return new_print;
}