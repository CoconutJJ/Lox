#ifndef TOKEN_H
#define TOKEN_H

#include "../types.h"

struct token *tokenize(char *code);
void destroy_token_list(struct token *list);

#endif