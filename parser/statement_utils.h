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
#ifndef STATEMENT_UTIL_H
#define STATEMENT_UTIL_H

#include "../types.h"

struct while_statement* create_while_stmt();

struct ifelse_statement* create_ifelse_stmt();

struct assignment_statement* create_assignment_stmt();

struct declaration_statement* create_declaration_stmt();

struct print_statement* create_print_stmt();
#endif