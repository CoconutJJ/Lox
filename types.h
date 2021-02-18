#ifndef TYPES_H

#define TYPES_H

#define AS_BIN_OP_PTR(r) ((EXPR_BIN_OP *)(r))
#define AS_UNR_OP_PTR(r) ((EXPR_UNR_OP *)(r))
#define AS_EXPR_OP_PTR(r) ((EXPR_OP *)(r))

typedef enum _error_t{

    SYNTAX_ERROR,
    RUNTIME_ERROR

} ERROR_T;


typedef struct _error_entry
{
    char * msg;
    int line;
    ERROR_T type;

} ERROR_ENTRY;


typedef struct _token TOKEN;

typedef enum _token_t_
{
    FUNC,
    RETURN,
    NIL,
    IF,
    ELSE,
    ELSEIF,
    WHILE,
    FOR,
    VAR,
    EQUAL,
    GREATER,
    LESS,
    GREATER_EQ,
    LESS_EQ,
    EQUAL_EQUAL,
    BANG_EQUAL,
    BANG,
    AND,
    OR,
    TRUE,
    FALSE,
    PLUS,
    MINUS,
    MULTIPLY,
    SLASH,
    SEMICOLON, 
    COMMA,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    IDENTIFIER,
    NUMBER,
    STRING

} TOKEN_T;

typedef struct _token
{
    TOKEN_T t;
    TOKEN * next;
    int line;
    char value[];
} TOKEN;

typedef enum _op_v
{
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
    EXPR_V_FALSE

} EXPR_V;

/**
 * OP Types
 * 
 * EXPR_T_BINARY - Binary Operators
 * EXPR_T_UNARY - Unary Operators
 * 
 */
typedef enum _op_t
{

    EXPR_T_BINARY,
    EXPR_T_UNARY,
    EXPR_T_STRING,
    EXPR_T_NUMBER,
    EXPR_T_BOOL

} EXPR_T;

/**
 * WARNING! PLEASE DO NOT INITIALIZE THIS ALONE.
 * 
 * DO NOT EVER DO malloc(sizeof(EXPR_OP))!!
 * 
 * IT IS NOT ONLY A USELESS STRUCT BY ITSELF BUT WILL CAUSE SEGFAULT IF
 * CASTED TO EXPR_BIN_OP OR EXPR_UNR_OP.
 */
typedef struct _op
{
    EXPR_T expr_t;
} EXPR_OP;

typedef struct _expr_bin_op
{
    EXPR_OP _expr_op; // this property makes it child of EXPR_OP
    EXPR_OP *left;
    EXPR_OP *right;
    EXPR_V op;

} EXPR_BIN_OP;

typedef struct _expr_str
{
    EXPR_OP _expr_op;
    char *data;
} EXPR_STR;

typedef struct _expr_number
{
    EXPR_OP _expr_op;
    double data;
} EXPR_NUM;

typedef struct _expr_bool
{
    EXPR_OP _expr_op;
    int data;
} EXPR_BOOL;

typedef struct _expr_unr_op
{
    EXPR_OP _expr_op;
    EXPR_OP *body;
    EXPR_V op;

} EXPR_UNR_OP;


typedef struct statement_t
{
    enum
    {
        E_IFELSE_STATEMENT,
        E_WHILE_STATEMENT,
        E_FOR_STATEMENT
    } type;
    char pad[4]; // 4 bytes are added by compiler here to comply with pointer alignment 
    struct statement_t *next;
} STATEMENT;

typedef struct _ifelse_statement
{

    struct statement_t _statment_;
    EXPR_OP *cond_expr;
    struct statement_t *if_clause;
    struct statement_t *else_clause;

} IFELSE_STATEMENT;

typedef struct _while_statement
{

    struct statement_t _statement_;
    EXPR_OP *cond_expr;
    struct statement_t *body;

} WHILE_STATEMENT;

typedef struct _for_statement
{

} FOR_STATEMENT;

typedef struct _assignment_statement
{
    
    char * identifier_name;
    EXPR_OP * identifier_value;
    
} ASSIGNMENT_STATEMENT;


#endif