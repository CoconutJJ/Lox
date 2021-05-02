#include "parse_errors.h"

#include <stdio.h>
#include <stdlib.h>

#include "../types.h"

static int is_panic_mode = 0;

int is_panic() { return is_panic_mode; }

void panic(TOKEN **current) {
        is_panic_mode = 1;

        while ((*current)) {
                switch ((*current)->t) {
                case IF:
                case WHILE:
                        goto panic_end;
                        break;
                default:
                        (*current) = (*current)->next;
                        break;
                }
        }

panic_end:

        return;
}

void register_error(ERROR_T type, char *msg, TOKEN **current) {
        if (is_panic()) return;

        switch (type) {
        case SYNTAX_ERROR:
                if (!(*current)) {
                        fprintf(stderr, "syntax error: %s\n", msg);
                        fprintf(stderr, "Unexpected end of input\n");
                } else {
                        fprintf(stderr, "syntax error: line %d: %s\n",
                                (*current)->line, msg);
                }

                break;
        default:
                fprintf(stderr, "register_error: type not implemented");
                exit(EXIT_FAILURE);
        }
}

void parse_error(char *msg, int line) {
        if (is_panic()) exit(EXIT_FAILURE);

        fprintf(stderr, "parse error: line %d: %s", line, msg);
        exit(EXIT_FAILURE);
}