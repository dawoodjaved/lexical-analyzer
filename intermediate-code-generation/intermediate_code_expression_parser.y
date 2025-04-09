%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int yylex();
void yyerror(const char *s);

typedef struct AST {
    char *token;
    struct AST *left;
    struct AST *right;
} AST;

AST* createNode(const char *token, AST *left, AST *right) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->token = strdup(token);
    node->left = left;
    node->right = right;
    return node;
}

int tempCount = 0;

char* generateCode(AST* root) {
    if (!root) return NULL;

    if (!root->left && !root->right) {
        return root->token;
    }

    char* leftVal = generateCode(root->left);
    char* rightVal = generateCode(root->right);

    char* tmp = (char*)malloc(10);
    sprintf(tmp, "t%d", tempCount++);

    printf("%s = %s %s %s\n", tmp, leftVal, root->token, rightVal);
    return tmp;
}
%}

%union {
    char* str;
    struct AST* node;
}

%start S

%token <str> ID
%token <str> NUM
%token NL

%left '+' '-'
%left '*' '/'

%type <node> E

%%

S : E NL {
        printf("Three-Address Code:\n");
        char* result = generateCode($1);
        printf("; Final result in %s\n", result);
    }
  ;

E : E '+' E {
        $$ = createNode("+", $1, $3);
    }
  | E '-' E {
        $$ = createNode("-", $1, $3);
    }
  | E '*' E {
        $$ = createNode("*", $1, $3);
    }
  | E '/' E {
        $$ = createNode("/", $1, $3);
    }
  | '(' E ')' {
        $$ = $2;
    }
  | ID {
        $$ = createNode($1, NULL, NULL);
    }
  | NUM {
        $$ = createNode($1, NULL, NULL);
    }
  ;

%%

int main() {
    printf("Enter an expression:\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int yylex() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == EOF) return 0;

    if (c == '\n') return NL;

    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
        return c;
    }

    if (isalpha(c)) {
        char buffer[128]; int i = 0;
        buffer[i++] = c;
        while ((c = getchar()) != EOF && isalnum(c)) {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        ungetc(c, stdin);
        yylval.str = strdup(buffer);
        return ID;
    }

    if (isdigit(c)) {
        char buffer[128]; int i = 0;
        buffer[i++] = c;
        while ((c = getchar()) != EOF && isdigit(c)) {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        ungetc(c, stdin);
        yylval.str = strdup(buffer);
        return NUM;
    }

    return c;
}
