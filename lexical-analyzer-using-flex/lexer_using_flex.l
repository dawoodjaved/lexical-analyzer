%{
#include <stdio.h>
extern FILE *yyin;
%}

%%

[ \t\n]+                                     ; /* Ignore whitespace */

"int"|"float"|"if"|"else"|"while"|"return"   { printf("[KEYWORD: %s]\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*                       { printf("[IDENTIFIER: %s]\n", yytext); }

[0-9]+(\.[0-9]+)?                            { printf("[NUMBER: %s]\n", yytext); }

[\+\-\*/=(){};]                              { printf("[OPERATOR/PUNCT: %s]\n", yytext); }

.                                            { /* Ignore unrecognized characters */ }

%%

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Error: Unable to open file %s\n", argv[1]);
            return 1;
        }
        yyin = fp;
    } else {
        printf("Reading from stdin. Press Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) to end:\n");
    }

    yylex();
    return 0;
}
