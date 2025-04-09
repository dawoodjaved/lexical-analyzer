%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s){
    fprintf(stderr, "Error: %s\n", s);
}
%}

%token ID

%%
S : ID { printf("Valid variable.\n"); }
  ;

%%

int main() {
    printf("Enter a string to check for valid variable:\n");
    yyparse();
    return 0;
}
