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

#define AS_BIN_OP_PTR(r) ((EXPR_BIN_OP *)(r))
#define AS_UNR_OP_PTR(r) ((EXPR_UNR_OP *)(r))
#define AS_EXPR_OP_PTR(r) ((EXPR_OP *)(r))

typedef enum _error_t {

        SYNTAX_ERROR,
        RUNTIME_ERROR

} ERROR_T;

typedef struct _error_entry {
        char *  msg;
        int     line;
        ERROR_T type;

} ERROR_ENTRY;

typedef struct _token TOKEN;

typedef enum _token_t_ {
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

} TOKEN_T;

typedef struct _token {
        /**
         * Type of token
         */
        TOKEN_T t;

        /**
         * Next Token
         */
        TOKEN *next;

        /**
         * Line No.
         */
        int line;

        /**
         * Token value. For STRING and NUMBER tokens only.
         */
        char value[];
} TOKEN;

typedef enum _op_v {
        EXPR_V_AND,
        EXPR_V_OR,
        EXPR_V_PLUS,
        EXPR_V_MINUS,
        EXPR_V_DIVIDE,
        EXPR_V_MULTIPLY,
        EXPR_V_NOT,
        EXPR_V_GREATER,
        EXPR_V_LESS,
        EXPR_V_GREATER_EQ,
        EXPR_V_LESS_EQ,
        EXPR_V_EQUAL_EQUAL,
        EXPR_V_BANG_EQUAL,
        EXPR_V_LEFT_PAREN,
        EXPR_V_RIGHT_PAREN,
        EXPR_V_NUMBER,
        EXPR_V_STRING,
        EXPR_V_TRUE,
        EXPR_V_FALSE,
        EXPR_V_VAR
} EXPR_V;

/**
 * OP Types
 *
 * EXPR_T_BINARY - Binary Operators
 * EXPR_T_UNARY - Unary Operators
 *
 */
typedef enum _op_t {

        EXPR_T_BINARY,
        EXPR_T_UNARY,
        EXPR_T_STRING,
        EXPR_T_NUMBER,
        EXPR_T_BOOL,
        EXPR_T_VAR

} EXPR_T;

/**
 * WARNING! PLEASE DO NOT INITIALIZE THIS ALONE.
 *
 * DO NOT EVER DO malloc(sizeof(EXPR_OP))!!
 *
 * IT IS NOT ONLY A USELESS STRUCT BY ITSELF BUT WILL CAUSE SEGFAULT IF
 * CASTED TO EXPR_BIN_OP OR EXPR_UNR_OP.
 */
typedef struct _op {
        EXPR_T expr_t;
        int line;
} EXPR_OP;

typedef struct _expr_bin_op {
        EXPR_OP  _expr_op;  // this property makes it child of EXPR_OP
        EXPR_OP *left;
        EXPR_OP *right;
        EXPR_V   op;

} EXPR_BIN_OP;

typedef struct _expr_str {
        EXPR_OP _expr_op;
        char *  data;
} EXPR_STR;

typedef struct _expr_number {
        EXPR_OP _expr_op;
        double  data;
} EXPR_NUM;

typedef struct _expr_bool {
        EXPR_OP _expr_op;
        int     data;
} EXPR_BOOL;
typedef struct _expr_var {
        EXPR_OP _expr_op;
        char *  var;
        /* data */
} EXPR_VAR;

typedef struct _expr_unr_op {
        EXPR_OP  _expr_op;
        EXPR_OP *body;
        EXPR_V   op;

} EXPR_UNR_OP;

typedef struct statement_t {
        enum {
                E_IFELSE_STATEMENT,
                E_WHILE_STATEMENT,
                E_FOR_STATEMENT,
                E_ASSIGNMENT_STATEMENT,
                E_DECLARATION_STATEMENT,
                E_PRINT_STATEMENT
        } type;
        int                 line;
        struct statement_t *next;
} STATEMENT;

typedef struct _ifelse_statement {
        struct statement_t  _statment_;
        EXPR_OP *           cond_expr;
        struct statement_t *if_clause;
        struct statement_t *else_clause;

} IFELSE_STATEMENT;

typedef struct _while_statement {
        struct statement_t  _statement_;
        EXPR_OP *           cond_expr;
        struct statement_t *body;

} WHILE_STATEMENT;

// typedef struct _for_statement {
// } FOR_STATEMENT;

typedef struct _assignment_statement {
        struct statement_t _statement_;
        char *             identifier_name;
        EXPR_OP *          identifier_value;

} ASSIGNMENT_STATEMENT;

typedef struct _declaration_statement {
        struct statement_t _statement_;
        char *             identifier_name;
        EXPR_OP *          identifier_value;

} DECLARATION_STATEMENT;

typedef struct _print_statement {
        struct statement_t _statement_;
        EXPR_OP *          value;

} PRINT_STATEMENT;

typedef struct hashcontainer {
        char *key;
        void *item;
        int   deleted;

} HashContainer;

typedef struct hashtable {
        HashContainer *table;
        int            size;
        int            filled;

} HashTable;

typedef struct _environment {
        HashTable *          table;
        struct _environment *next;
        struct _environment *prev;

} ENVIRONMENT;

#endif