

#include <stdlib.h>

#include "../types.h"

WHILE_STATEMENT* create_while_stmt() {
        WHILE_STATEMENT* new_while = malloc(sizeof(WHILE_STATEMENT));

        new_while->_statement_.next = NULL;
        new_while->_statement_.type = E_WHILE_STATEMENT;
        new_while->body             = NULL;
        new_while->cond_expr        = NULL;

        return new_while;
}

IFELSE_STATEMENT* create_ifelse_stmt() {
        IFELSE_STATEMENT* new_ifelse = malloc(sizeof(IFELSE_STATEMENT));

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

        new_assignment->_statement_.next = NULL;
        new_assignment->_statement_.type = E_ASSIGNMENT_STATEMENT;
        new_assignment->identifier_name  = NULL;
        new_assignment->identifier_value = NULL;

        return new_assignment;
}

DECLARATION_STATEMENT* create_declaration_stmt() {
        DECLARATION_STATEMENT* new_declaration =
            malloc(sizeof(DECLARATION_STATEMENT));

        new_declaration->_statement_.next = NULL;
        new_declaration->_statement_.type = E_DECLARATION_STATEMENT;
        new_declaration->identifier_name  = NULL;
        new_declaration->identifier_value = NULL;

        return new_declaration;
}