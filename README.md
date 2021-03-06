# Lox-like Toy Language 

## About

This project is my first dedicated attempt into a creating a recursive descent
interpreted language. It is full of questionable design decisions.

# Language Syntax
```
<STATEMENT>     =   | <STATEMENT> <STATEMENT>
                    | if (<EXPR>) { <STATEMENT> } <ENDIF>
                    | while (<EXPR>) { <STATEMENT> }
                    | var <IDENTIFIER> = <EXPR>
                    | <IDENTIFIER> = <EXPR>
                    | ""

<ENDIF>         =   | else { <STATEMENT> }
                    | else if { <STATEMENT> } <ENDIF>
                    | ""

<EXPR>          =   | <EXPR-EQUAL> == <EXPR-EQUAL>
                    | <EXPR-EQUAL> != <EXPR-EQUAL>
                    | <EXPR-EQUAL>

<EXPR-EQUAL>    =   | <EXPR-CMP> > <EXPR-CMP>
                    | <EXPR-CMP> >= <EXPR-CMP>
                    | <EXPR-CMP> < <EXPR-CMP>
                    | <EXPR-CMP> <= <EXPR-CMP>
                    | <EXPR-CMP>

<EXPR-CMP>      =   | <EXPR-OR> || <EXPR-OR>
                    | <EXPR-OR>

<EXPR-OR>       =   | <EXPR-AND> && <EXPR-AND>
                    | <EXPR-AND>

<EXPR-AND>      =   | <EXPR-SUM> + <EXPR-SUM>
                    | <EXPR-SUM> - <EXPR-SUM>
                    | <EXPR-SUM>

<EXPR-SUM>      =   | <EXPR-PROD> * <EXPR-PROD>
                    | <EXPR-PROD> / <EXPR-PROD>
                    | <EXPR-PROD>

<EXPR-PROD>     =   | <EXPR-UNR>

<EXPR-UNR>      =   | ! <EXPR-UNR>
                    | - <EXPR-UNR>
                    | <EXPR-PRIMARY>

<EXPR-PRIMARY>  =   | ( <EXPR> )
                    | <STRING>
                    | <NUMBER>
                    | TRUE
                    | FALSE
                    | <IDENTIFIER>
```

## How to compile and run

This project is trivial to compile

Simply run
```
$ make
$ make clean
```
in the project's root directory and then run

```
$ ./interp <filename>
```

to run a source file.

Here is sample script you can try out:

```
// Here is a comment

var a = 3 
var b = 2

if (a > b) {

    if (2 * a >= b) {
        print("A")
    }

    a = a * b
    
    print(a)

} else {

    print("C")

}

while (a * b < 2000) {

    print("D")

    a = a + 1
    b = b + 1
}
```


## Credits

Thank you so much to Bob Nystrom and his excellent Crafting Interpreters guide 

https://craftinginterpreters.com/