all: parse_errors.o parse_expression.o expression_utils.o statement.o token.o utils.o

parse_errors.o: error/parse_errors.c
	gcc -c error/parse_errors.c -o parse_errors.o

evaluate_expr.o: evaluate/evaluate_expr.c
	gcc -c evaluate/evaluate_expr.c -o evaluate_expr.o

parse_expression.o: parser/expression.c
	gcc -c parser/expression.c -o parse_expression.o

expression_utils.o: parser/expression_utils.c
	gcc -c parser/expression_utils.c -o expression_utils.o

statement.o: parser/statement.c
	gcc -c parser/statement.c -o statement.o

token.o: token/tokenizer.c
	gcc -c token/tokenizer.c -o tokenizer.o

utils.o: utils/utils.c
	gcc -c utils/utils.c -o utils.o

.PHONY: clean

clean: 
	rm tokenizer.o parse_expression.o evaluate_expr.o parse_errors.o