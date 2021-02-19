#include <stdio.h>
#include <stdlib.h>

#include "parser/statement.h"
#include "token/token.h"
#include "types.h"
int main(int argc, char **argv) {
        if (argc == 1) {
                char buf[500];
                while (fgets(buf, 500, stdin) != NULL) {
                        TOKEN *           current = tokenize(buf);
                        TOKEN *           head    = current;
                        STATEMENT *       code    = parse_statements(&current);
                        IFELSE_STATEMENT *s       = (IFELSE_STATEMENT *)code;

                        destroyTokenList(head);

                        printf("%p", s);
                }

        } else if (argc == 2) {
        }
}
