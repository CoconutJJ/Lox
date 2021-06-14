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
#ifndef TYPES_H
#define TYPES_H

#define AS_BIN_OP_PTR(r) ((struct expr_bin_op *)(r))
#define AS_UNR_OP_PTR(r) ((struct expr_unr_op *)(r))
#define AS_struct expr_op_PTR(r)((struct expr_op *)(r))

#define CAST_PROP(type, var, property) (((type)var)->property)

typedef enum error_t {
        PARSE_ERROR,
        SYNTAX_ERROR,
        RUNTIME_ERROR

};

typedef struct _error_entry {
        char *msg;
        int line;
        enum error_t type;
};

enum token_t {
        AND,
        BANG,
        BANG_EQUAL,
        COMMA,
        ELSE,
        ELSEIF,
        EQUAL,
        EQUAL_EQUAL,
        FALSE,
        FOR,
        FUNC,
        GREATER,
        GREATER_EQ,
        IDENTIFIER,
        IF,
        LEFT_BRACE,
        LEFT_PAREN,
        LESS,
        LESS_EQ,
        MINUS,
        MULTIPLY,
        NIL,
        NUMBER,
        OR,
        PLUS,
        PRINT,
        RETURN,
        RIGHT_BRACE,
        RIGHT_PAREN,
        SEMICOLON,
        SLASH,
        STRING,
        TRUE,
        VAR,
        WHILE,

};

struct token {
        /**
         * Type of token
         */
        enum token_t t;

        /**
         * Next Token
         */
        struct token *next;

        /**
         * Token value. For STRING and NUMBER tokens only.
         */
        char *value;

        /**
         * Line No.
         */
        int line;
};

enum op_v {
        EXPR_V_AND,
        EXPR_V_BANG_EQUAL,
        EXPR_V_DIVIDE,
        EXPR_V_EQUAL_EQUAL,
        EXPR_V_FALSE,
        EXPR_V_GREATER,
        EXPR_V_GREATER_EQ,
        EXPR_V_LEFT_PAREN,
        EXPR_V_LESS,
        EXPR_V_LESS_EQ,
        EXPR_V_MINUS,
        EXPR_V_MULTIPLY,
        EXPR_V_NOT,
        EXPR_V_NUMBER,
        EXPR_V_OR,
        EXPR_V_PLUS,
        EXPR_V_RIGHT_PAREN,
        EXPR_V_STRING,
        EXPR_V_TRUE,
        EXPR_V_VAR
};

/**
 * OP Types
 *
 * EXPR_T_BINARY - Binary Operators
 * EXPR_T_UNARY - Unary Operators
 *
 */
enum op_t {
        EXPR_T_BINARY = 0b1,
        EXPR_T_BOOL = 0b10,
        EXPR_T_NUMBER = 0b100,
        EXPR_T_STRING = 0b1000,
        EXPR_T_UNARY = 0b10000,
        EXPR_T_VAR = 0b100000
};

/**
 * WARNING! PLEASE DO NOT INITIALIZE THIS ALONE.
 *
 * DO NOT EVER DO malloc(sizeof(struct expr_op))!!
 *
 * IT IS NOT ONLY A USELESS STRUCT BY ITSELF BUT WILL CAUSE SEGFAULT IF
 * CASTED TO struct expr_bin_op  OR struct expr_unr_op.
 */
struct expr_op {
        enum op_t expr_t;
        int line;
};

struct expr_bin_op {
        struct expr_op
            _expr_op;  // this property makes it child of struct expr_op
        struct expr_op *left;
        struct expr_op *right;
        enum op_v op;
};

struct expr_str {
        struct expr_op _expr_op;
        char *data;
};

struct expr_number {
        struct expr_op _expr_op;
        double data;
};

struct expr_bool {
        struct expr_op _expr_op;
        int data;
};
struct expr_var {
        struct expr_op _expr_op;
        char *var;
        /* data */
};

struct expr_unr_op {
        struct expr_op _expr_op;
        struct expr_op *body;
        enum op_v op;
};

struct statement_t {
        struct statement_t *next;
        int line;

        enum {
                E_IFELSE_STATEMENT,
                E_WHILE_STATEMENT,
                E_FOR_STATEMENT,
                E_ASSIGNMENT_STATEMENT,
                E_DECLARATION_STATEMENT,
                E_PRINT_STATEMENT
        } type;
};

struct ifelse_statement {
        struct statement_t _statment_;
        struct expr_op *cond_expr;
        struct statement_t *if_clause;
        struct statement_t *else_clause;
};

struct while_statement {
        struct statement_t _statement_;
        struct expr_op *cond_expr;
        struct statement_t *body;
};

// typedef struct _for_statement {
// } FOR_STATEMENT;

struct assignment_statement {
        struct statement_t _statement_;
        char *identifier_name;
        struct expr_op *identifier_value;
};

struct declaration_statement {
        struct statement_t _statement_;
        char *identifier_name;
        struct expr_op *identifier_value;
};

struct print_statement {
        struct statement_t _statement_;
        struct expr_op *value;
};

struct hashcontainer {
        char *key;
        void *item;
        int deleted;
};

struct hashtable {
        struct hashcontainer *table;
        int size;
        int filled;
};

struct environment {
        struct hashtable *table;
        struct environment *next;
        struct environment *prev;

};

#endif