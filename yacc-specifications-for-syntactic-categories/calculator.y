%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%
S : E { printf("Result = %d\n", $1); }
  ;

E : E '+' E { $$ = $1 + $3; }
  | E '-' E { $$ = $1 - $3; }
  | E '*' E { $$ = $1 * $3; }
  | E '/' E { 
        if($3 == 0) {
            yyerror("Division by zero!");
            YYABORT;
        } else {
            $$ = $1 / $3;
        }
    }
  | '(' E ')' { $$ = $2; }
  | NUMBER    { $$ = $1; }
  ;

%%

int main() {
    printf("Enter an expression to evaluate:\n");
    yyparse();
    return 0;
}
