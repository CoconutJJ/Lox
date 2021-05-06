#ifndef TOKEN_H
#define TOKEN_H

#include "../types.h"

TOKEN *tokenize(char *code);
void destroy_token_list(TOKEN * list);

#endif