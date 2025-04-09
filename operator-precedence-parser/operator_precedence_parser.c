#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// We'll define a quick operator precedence table
// Return < 0 if op1 < op2, > 0 if op1 > op2, 0 if same precedence
int precedence(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '(': return 0; // special case
    }
    return -1; // for others
}

// We'll define a small stack
char opStack[100];
int top = -1;

void push(char c) {
    opStack[++top] = c;
}

char pop() {
    return opStack[top--];
}

char peek() {
    if(top >= 0) return opStack[top];
    return '\0';
}

// We'll treat single letters/numbers as operands
// We'll parse an expression e.g. "a + b * c"
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int main() {
    char expr[200];
    printf("Enter an expression: ");
    fgets(expr, 200, stdin);
    expr[strcspn(expr, "\n")] = '\0';

    char output[200];
    int outPos = 0;

    push('$');  // sentinel

    int i=0;
    while(expr[i] != '\0') {
        if(isspace(expr[i])) {
            i++;
            continue;
        }
        if(isalnum(expr[i])) {
            // operand
            output[outPos++] = expr[i];
            i++;
        } else if(expr[i] == '(') {
            push('(');
            i++;
        } else if(expr[i] == ')') {
            // pop until '('
            while(peek() != '(' && peek() != '$') {
                output[outPos++] = pop();
            }
            if(peek() == '(') {
                pop(); // remove '('
            } else {
                printf("Mismatched parentheses!\n");
                return 1;
            }
            i++;
        } else if(isOperator(expr[i])) {
            // check precedence
            while(isOperator(peek()) && 
                  precedence(peek()) >= precedence(expr[i])) {
                output[outPos++] = pop();
            }
            push(expr[i]);
            i++;
        } else {
            printf("Invalid character: %c\n", expr[i]);
            return 1;
        }
    }
    // pop remaining
    while(peek() != '$') {
        if(peek() == '(' || peek() == ')') {
            printf("Mismatched parentheses!\n");
            return 1;
        }
        output[outPos++] = pop();
    }
    output[outPos] = '\0';

    printf("Valid expression. Postfix form: %s\n", output);
    return 0;
}
