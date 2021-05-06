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

void register_error(ERROR_T type, char *msg, int line) {

        switch (type) {
        case SYNTAX_ERROR:
                fprintf(stderr, "syntax error: line %d: %s\n", line, msg);
                exit(EXIT_FAILURE);
                break;
        case RUNTIME_ERROR:
                fprintf(stderr, "runtime error: line %d: %s\n", line, msg);
                exit(EXIT_FAILURE);
        }
}

void parse_error(char *msg, int line) {

        fprintf(stderr, "parse error: line %d: %s", line, msg);
        exit(EXIT_FAILURE);
}