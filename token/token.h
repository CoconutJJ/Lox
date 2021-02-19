#ifndef TOKEN_H
#define TOKEN_H

#include "../types.h"

TOKEN *tokenize(char *code);
void destroyTokenList(TOKEN *head);

#endif