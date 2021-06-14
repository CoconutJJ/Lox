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
#include <string.h>

#include "../error/errors.h"
#include "../types.h"
#include "../utils/utils.h"

struct token *start = NULL;
struct token *end = NULL;

int token_at_end(char *code, int *c) { return code[*c] == '\0'; }

int match(char *code, int *c, char match) {
        if (token_at_end(code, c)) return 0;

        if (code[*c + 1] == match) {
                (*c)++;
                return 1;
        }
        return 0;
}

char peek(char *code, int *c) {
        if (token_at_end(code, c)) return '\0';

        return code[*c + 1];
}

int is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_num(char c) { return (c >= '0' && c <= '9'); }

void add_token(enum token_t t, char *value, int line) {
        struct token *new_token = malloc(sizeof(struct token));

        if (!new_token) {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        new_token->t = t;
        new_token->line = line;
        new_token->next = NULL;
        new_token->value = NULL;

        if (value) {
                new_token->value = malloc(strlen(value) + 1);

                if (!new_token->value) {
                        perror("malloc");
                        exit(EXIT_FAILURE);
                }

                strcpy(new_token->value, value);
        }

        if (start) {
                end->next = new_token;
                end = end->next;
        } else {
                start = new_token;
                end = start;
        }

        return;
}

enum token_t match_identifier(char *ident) {
        if (strcmp(ident, "if") == 0) {
                return IF;
        }

        if (strcmp(ident, "print") == 0) {
                return PRINT;
        }

        if (strcmp(ident, "else") == 0) {
                return ELSE;
        }

        if (strcmp(ident, "while") == 0) {
                return WHILE;
        }

        if (strcmp(ident, "for") == 0) {
                return FOR;
        }

        if (strcmp(ident, "func") == 0) {
                return FUNC;
        }

        if (strcmp(ident, "return") == 0) {
                return RETURN;
        }

        if (strcmp(ident, "var") == 0) {
                return VAR;
        }

        if (strcmp(ident, "TRUE") == 0) {
                return TRUE;
        }

        if (strcmp(ident, "FALSE") == 0) {
                return FALSE;
        }

        if (strcmp(ident, "NULL") == 0) {
                return NIL;
        }

        return -1;
}

void identifier(char *code, int *c, int line) {
        char *start = &code[*c];

        while (is_alpha(code[*c + 1]) || is_num(code[*c + 1])) {
                (*c)++;
        }

        char *end = &(code[*c]);

        char ident[end - start + 2];

        nstrcp(ident, start, end - start + 1);

        enum token_t id = match_identifier(ident);

        add_token(id != -1 ? id : IDENTIFIER, id != -1 ? NULL : ident, line);
}

void unescape_string(char *str, char *buf, int line) {
        char *start = str;

        /**
         * Employ a cursor that will always point to the next byte we can write
         * to in buf.
         */
        char *cursor = buf;

        char *esc;

        while ((esc = strchr(start, '\\')) != NULL) {
                /**
                 * Only copy bytes from previous upto but not including the
                 * backslash
                 */
                strncpy(cursor, start, esc - start);
                cursor += (esc - start);

                /**
                 * If *(esc + 1) is the null byte, then it will be an invalid
                 * escape sequence since this implies '\' is the last character
                 * in the string and there is no '\' before it to esape it.
                 *
                 * Handled safely by default case
                 */

                switch (*(esc + 1)) {
                case 'a':
                        *cursor = 0x07;
                        break;
                case 'b':
                        *cursor = 0x08;
                        break;
                case 'e':
                        *cursor = 0x1B;
                        break;
                case 'f':
                        *cursor = 0x1C;
                        break;
                case 'n':
                        *cursor = 0x0A;
                        break;
                case 'r':
                        *cursor = 0x0D;
                        break;
                case 'v':
                        *cursor = 0x0B;
                        break;
                case '\\':
                        *cursor = 0x5C;
                        break;
                case '\'':
                        *cursor = 0x27;
                        break;
                case '\"':
                        *cursor = 0x22;
                        break;
                case '\?':
                        *cursor = 0x3F;
                        break;
                default:
                        register_error(PARSE_ERROR, "unknown escape sequence",
                                       line);
                        break;
                }

                cursor++;

                /**
                 * Increment esc pointer by 2, one for escape code and another
                 * to mark start of string. Set result to start pointer so next
                 * iteration start search from after the escape sequence.
                 */
                start = esc + 2;
        }

        strcpy(cursor, start);
}

void string(char *code, int *c, int line) {
        char *start = &(code[*c]) + 1;

        while (peek(code, c) != '"') {
                if (token_at_end(code, c))
                        register_error(PARSE_ERROR,
                                       "expected end-quote from string", line);

                (*c)++;
        }
        char *end = &code[*c];

        char str[end - start + 2];
        char unescaped_str[end - start + 2];

        nstrcp(str, start, end - start + 1);

        /**
         * The unescaped string can only be shorter than the escaped string,
         * therefore it is safe to pass a buffer the same size as the original
         * string.
         */
        unescape_string(str, unescaped_str, line);

        add_token(STRING, unescaped_str, line);

        (*c)++;
}

void number(char *code, int *c, int line) {
        int hasDecimal = 0;
        char *start = &code[*c];

        while (1) {
                if (is_num(code[*c])) {
                        (*c)++;
                        continue;
                }

                if (code[*c] == '.') {
                        if (!hasDecimal) {
                                hasDecimal = 1;
                                (*c)++;
                                continue;
                        }
                }
                break;
        }
        (*c)--;
        char *end = &code[*c];

        char num[end - start + 2];

        nstrcp(num, start, end - start + 1);

        add_token(NUMBER, num, line);
}

void destroy_token_list(struct token *list) {
        while (list) {
                struct token *next = list->next;

                if (list->value) free(list->value);

                free(list);

                list = next;
        }
}

struct token *tokenize(char *code) {
        int c = 0;
        int line = 1;
        while (!token_at_end(code, &c)) {
                switch (code[c]) {
                case '+':
                        add_token(PLUS, NULL, line);
                        break;
                case '-':
                        add_token(MINUS, NULL, line);
                        break;
                case '*':
                        add_token(MULTIPLY, NULL, line);
                        break;
                case '/':
                        if (match(code, &c, '/')) {
                                while (peek(code, &c) != '\n') c++;
                        } else {
                                add_token(SLASH, NULL, line);
                        }
                        break;
                case ';':
                        add_token(SEMICOLON, NULL, line);
                        break;
                case ',':
                        add_token(COMMA, NULL, line);
                        break;
                case '&':

                        if (match(code, &c, '&')) {
                                add_token(AND, NULL, line);
                        } else {
                                register_error(
                                    PARSE_ERROR,
                                    "unknown symbol &. Did you mean && "
                                    "?",
                                    line);
                        }

                        break;
                case '|':
                        if (match(code, &c, '|')) {
                                add_token(OR, NULL, line);
                        } else {
                                register_error(
                                    PARSE_ERROR,
                                    "unknown symbol |. Did you mean || "
                                    "?",
                                    line);
                        }

                        break;
                case '!':
                        add_token(match(code, &c, '=') ? BANG_EQUAL : BANG,
                                  NULL, line);
                        break;
                case '(':
                        add_token(LEFT_PAREN, NULL, line);
                        break;
                case ')':
                        add_token(RIGHT_PAREN, NULL, line);
                        break;
                case '{':
                        add_token(LEFT_BRACE, NULL, line);
                        break;
                case '}':
                        add_token(RIGHT_BRACE, NULL, line);
                        break;
                case '"':
                        string(code, &c, line);
                        break;
                case '=':
                        add_token(match(code, &c, '=') ? EQUAL_EQUAL : EQUAL,
                                  NULL, line);
                        break;
                case '>':
                        add_token(match(code, &c, '=') ? GREATER_EQ : GREATER,
                                  NULL, line);
                        break;
                case '<':
                        add_token(match(code, &c, '=') ? LESS_EQ : LESS, NULL,
                                  line);
                        break;
                case '\n':
                        line++;
                        break;
                case '\t':
                case ' ':
                        break;
                default:
                        if (is_alpha(code[c])) {
                                identifier(code, &c, line);
                        } else if (is_num(code[c])) {
                                number(code, &c, line);
                        } else {
                                register_error(PARSE_ERROR, "unknown symbol",
                                               line);
                        }
                }
                c++;
        }

        struct token *head = start;

        start = NULL;
        end = NULL;

        return head;
}