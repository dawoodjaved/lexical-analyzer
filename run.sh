

gcc manual_lexer.c -o manual_lexer
./manual_lexer testing_code.c





flex lexer_using_flex.l
gcc lex.yy.c -o lexer -lfl
./lexer testing_code.c






yacc -d arithmetic_expression_recognizer.y
flex arithmetic_expression_recognizer.l
gcc y.tab.c lex.yy.c -o arithmetic_expression_recognizer -lfl
./arithmetic_expression_recognizer





yacc -d valid_variable_recognizer.y
flex valid_variable_recognizer.l
gcc y.tab.c lex.yy.c -o valid_variable_recognizer -lfl
./valid_variable_recognizer



yacc -d calculator.y
flex calculator.l
gcc y.tab.c lex.yy.c -o calculator -lfl
./calculator



yacc -d abstract_syntax_tree_generator.y
flex abstract_syntax_tree_generator.l
gcc y.tab.c lex.yy.c -o abstract_syntax_tree_generator -lfl
./abstract_syntax_tree_generator









gcc first_and_follow.c -o first_and_follow
./first_and_follow


gcc ll1_parser.c -o ll1_parser
./ll1_parser



gcc operator_precedence_parser.c -o operator_precedence_parser
./operator_precedence_parser

bison -dy intermediate_code_expression_parser.y
gcc y.tab.c -o intermediate_code_expression_parser
./intermediate_code_expression_parser



gcc loop_unrolling.c -o loop_unrolling
./loop_unrolling






gcc constant_propagation.c -o constant_propagation
./constant_propagation constant_propagation.txt