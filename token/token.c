#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/parse_errors.h"
#include "../types.h"
#include "../utils/utils.h"

TOKEN *start = NULL;
TOKEN *end   = NULL;

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

void add_token(TOKEN_T t, char *value, int line) {
        int sz = sizeof(TOKEN);

        if (value) sz += strlen(value) + 1;

        TOKEN *new_token = malloc(sz);

        new_token->t    = t;
        new_token->line = line;
        new_token->next = NULL;

        if (value) strcpy(new_token->value, value);

        if (start) {
                end->next = new_token;
                end       = end->next;
        } else {
                start = new_token;
                end   = start;
        }

        return;
}

void destroyTokenList(TOKEN *head) { 
        while (head) {
                TOKEN * next = head->next;
                free(head);
                head = next;
        } 
        start = NULL;
        end = NULL;
}

TOKEN_T matchIdentifier(char *ident) {
        if (strcmp(ident, "if") == 0) {
                return IF;
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

        TOKEN_T id = matchIdentifier(ident);

        add_token(id != -1 ? id : IDENTIFIER, id != -1 ? NULL : ident, line);
}

void string(char *code, int *c, int line) {
        char *start = code + 1;

        while (peek(code, c) != '"') {
                if (token_at_end(code, c))
                        parse_error("expected end-quote from string", line);

                c++;
        }
        char *end = &code[*c];

        char str[end - start + 2];

        nstrcp(str, start, end - start + 1);

        add_token(STRING, str, line);

        c++;
}

void number(char *code, int *c, int line) {
        int   hasDecimal = 0;
        char *start      = &code[*c];

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

TOKEN *tokenize(char *code) {
        int c    = 0;
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
                                parse_error(
                                    "unknown symbol &. Did you mean && "
                                    "?",
                                    line);
                        }

                        break;
                case '|':
                        if (match(code, &c, '|')) {
                                add_token(OR, NULL, line);
                        } else {
                                parse_error(
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
                                parse_error("unknown symbol", line);
                        }
                }
                c++;
        }
        return start;
}