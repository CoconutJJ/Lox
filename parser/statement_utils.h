#ifndef STATEMENT_UTIL_H
#define STATEMENT_UTIL_H

#include "../types.h"

WHILE_STATEMENT * create_while_stmt();

IFELSE_STATEMENT * create_ifelse_stmt();

ASSIGNMENT_STATEMENT* create_assignment_stmt();

DECLARATION_STATEMENT* create_declaration_stmt();

PRINT_STATEMENT * create_print_stmt();
#endif