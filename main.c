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

#include "environment/environment.h"
#include "evaluate/evaluate_stmt.h"
#include "parser/statement.h"
#include "token/token.h"
#include "types.h"

int main(int argc, char **argv) {
        if (argc == 1) {
                fprintf(stderr, "error: no input files\n");
                exit(EXIT_FAILURE);
        }

        FILE *source = fopen(argv[1], "r");

        fseek(source, 0, SEEK_END);

        long source_sz = ftell(source);

        fseek(source, 0, SEEK_SET);

        char *buf = malloc(source_sz * sizeof(char) + 1);

        if (!buf) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        fread(buf, source_sz, 1, source);

        buf[source_sz] = '\0';

        fclose(source);

        struct token *head = tokenize(buf);

        struct token *curr = head;

        struct statement_t *code = parse_statements(&curr);

        destroy_token_list(head);

        struct environment *root_env = create_environment();

        evaluate_statements(code, root_env);

        destroy_environment(root_env);

        exit(EXIT_SUCCESS);
}
