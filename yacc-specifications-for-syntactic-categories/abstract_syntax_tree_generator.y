%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printAST(AST *root, int level) {
    if (!root) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", root->token);
    printAST(root->left, level + 1);
    printAST(root->right, level + 1);
}

extern int yylex();
void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
%}

%union {
    int num;
    struct AST* node;
}

%token <num> NUMBER
%type <node> S E

%left '+' '-'
%left '*' '/'

%%
S : E {
        printf("Abstract Syntax Tree:\n");
        printAST($1, 0);
    }
  ;

E : E '+' E  { $$ = createNode("+", $1, $3); }
  | E '-' E  { $$ = createNode("-", $1, $3); }
  | E '*' E  { $$ = createNode("*", $1, $3); }
  | E '/' E  { $$ = createNode("/", $1, $3); }
  | '(' E ')' { $$ = $2; }
  | NUMBER    {
        char temp[32];
        sprintf(temp, "%d", $1);
        $$ = createNode(temp, NULL, NULL);
    }
  ;

%%

int main() {
    printf("Enter an expression:\n");
    yyparse();
    return 0;
}
