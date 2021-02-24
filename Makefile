FLAGS=-g -Wall -Wextra -pedantic -O

all: parse_errors.o expression.o expression_utils.o statement.o statement_utils.o token.o utils.o main.c
	gcc $(FLAGS) parse_errors.o expression.o expression_utils.o statement.o statement_utils.o token.o utils.o main.c -o interp

parse_errors.o: error/parse_errors.c
	gcc $(FLAGS) -c error/parse_errors.c -o parse_errors.o

evaluate_expr.o: evaluate/evaluate_expr.c
	gcc $(FLAGS) -c evaluate/evaluate_expr.c -o evaluate_expr.o

expression.o: parser/expression.c
	gcc $(FLAGS) -c parser/expression.c -o expression.o

expression_utils.o: parser/expression_utils.c
	gcc $(FLAGS) -c parser/expression_utils.c -o expression_utils.o

statement_utils.o: parser/statement_utils.c
	gcc $(FLAGS) -c parser/statement_utils.c -o statement_utils.o

statement.o: parser/statement.c
	gcc $(FLAGS) -c parser/statement.c -o statement.o

token.o: token/token.c
	gcc $(FLAGS) -c token/token.c -o token.o

utils.o: utils/utils.c
	gcc $(FLAGS) -c utils/utils.c -o utils.o

.PHONY: clean

clean: 
	rm *.o