#include <stdio.h>
#include <stdlib.h>
#include "parse_errors.h"
#include "../types.h"

void panic(TOKEN **current) {

    while ((*current))
    {
        switch ((*current)->t)
        {
        case IF:
        case WHILE:
            break;
        default:
            (*current) = (*current)->next;
            break;
        }
    }
}

void register_error(ERROR_T type, char *msg, int line) {

    switch (type)
    {
    case SYNTAX_ERROR:
        fprintf(stderr, "syntax error: line %d: %s", line, msg);
        break;
    case RUNTIME_ERROR:
        fprintf(stderr, "runtime error: line %d: %s", line, msg);
        break;
    }

}

void parse_error(char *msg, int line)
{

    fprintf(stderr, "parse error: line %d: %s", line, msg);
    exit(EXIT_FAILURE);
}