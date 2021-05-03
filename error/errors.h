#ifndef PARSE_ERRORS_H
#define PARSE_ERRORS_H
#include "../types.h"

void parse_error(char *msg, int line);

void register_error(ERROR_T type, char *msg, int line);

void panic(TOKEN **current);

#endif