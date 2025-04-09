%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
int yyparse();
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%
S : E { printf("Valid arithmetic expression.\n"); }
  ;

E : E '+' E
  | E '-' E
  | E '*' E
  | E '/' E
  | '(' E ')'
  | NUMBER
  ;

%%

int main() {
    printf("Enter an expression:\n");
    yyparse();
    return 0;
}
