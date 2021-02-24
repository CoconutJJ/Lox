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
                }

        } else if (argc == 2) {

                FILE * source = fopen(argv[1], "r");

                fseek(source, 0, SEEK_END);

                long source_sz = ftell(source);
                
                fseek(source, 0, SEEK_SET);

                char * buf = malloc(source_sz * sizeof(char) + 1);

                fread(buf, source_sz, 1, source);

                buf[source_sz] = '\0';

                fclose(source);

                TOKEN * head = tokenize(buf);

                STATEMENT * code = parse_statements(&head);


                printf("%p", code);
                
        }
}
