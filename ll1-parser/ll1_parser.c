#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// A minimal token type
typedef enum {
    TOKEN_ID, TOKEN_NUM,
    TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_MUL, TOKEN_DIV,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_DOLLAR, // end of input
    TOKEN_INVALID
} TokenType;

// We'll just store the token type and maybe a value
typedef struct {
    TokenType type;
    char text[32];
} Token;

// Grammar (for reference):
// E -> T X
// X -> + T X | - T X | ε
// T -> F Y
// Y -> * F Y | / F Y | ε
// F -> ( E ) | id | num

// Define non-terminals as integers
#define E 1
#define X 2
#define T 3
#define Y 4
#define F 5

int parseTable[6][10];
#define MAX_RHS 10
int productions[15][MAX_RHS];
int prodCount = 10;

// We'll do a simple tokenizer for expressions
Token tokens[100];
int tokenCount = 0;
int currentTokenPos = 0;

// Stack for LL(1) parser
int stack[100];
int top = -1;

// Convert single char to a token type
TokenType charToTokenType(char c) {
    switch(c) {
        case '+': return TOKEN_PLUS;
        case '-': return TOKEN_MINUS;
        case '*': return TOKEN_MUL;
        case '/': return TOKEN_DIV;
        case '(': return TOKEN_LPAREN;
        case ')': return TOKEN_RPAREN;
        default:  return TOKEN_INVALID;
    }
}

// Tokenize the input string
void tokenize(const char* input) {
    tokenCount = 0;
    currentTokenPos = 0;
    int i = 0, len = strlen(input);

    while(i < len) {
        if(isspace(input[i])) {
            i++;
            continue;
        }
        if(isalpha(input[i])) {
            tokens[tokenCount].type = TOKEN_ID;
            int start = i;
            while(i < len && isalnum(input[i])) {
                i++;
            }
            int length = i - start;
            strncpy(tokens[tokenCount].text, input + start, length);
            tokens[tokenCount].text[length] = '\0';
            tokenCount++;
        }
        else if(isdigit(input[i])) {
            tokens[tokenCount].type = TOKEN_NUM;
            int start = i;
            while(i < len && isdigit(input[i])) {
                i++;
            }
            int length = i - start;
            strncpy(tokens[tokenCount].text, input + start, length);
            tokens[tokenCount].text[length] = '\0';
            tokenCount++;
        }
        else {
            TokenType t = charToTokenType(input[i]);
            if(t != TOKEN_INVALID) {
                tokens[tokenCount].type = t;
                tokens[tokenCount].text[0] = input[i];
                tokens[tokenCount].text[1] = '\0';
                tokenCount++;
                i++;
            } else {
                // Skip unrecognized character
                i++;
            }
        }
    }

    // Add the dollar token to mark end of input
    tokens[tokenCount].type = TOKEN_DOLLAR;
    strcpy(tokens[tokenCount].text, "$");
    tokenCount++;
}

Token getNextToken() {
    if(currentTokenPos < tokenCount) {
        return tokens[currentTokenPos++];
    }
    // If somehow we run out, return a DOLLAR token
    Token eofToken;
    eofToken.type = TOKEN_DOLLAR;
    strcpy(eofToken.text, "$");
    return eofToken;
}

// Parser stack operations
void push(int symbol) {
    stack[++top] = symbol;
}

int pop() {
    return stack[top--];
}

int peek() {
    return (top == -1) ? 0 : stack[top];
}

// Build the parse table
// parseTable[NonTerminal][Terminal] = production index
void initParseTable() {
    // Initialize with 0 => error
    memset(parseTable, 0, sizeof(parseTable));

    // E -> T X  (Production 1)
    // FIRST(T) is { ID, NUM, ( }, so:
    parseTable[E][TOKEN_ID]     = 1;
    parseTable[E][TOKEN_NUM]    = 1;
    parseTable[E][TOKEN_LPAREN] = 1;

    // X -> + T X (2), - T X (3), or ε (4)
    parseTable[X][TOKEN_PLUS]   = 2;
    parseTable[X][TOKEN_MINUS]  = 3;
    parseTable[X][TOKEN_RPAREN] = 4;  // Follow(X)
    parseTable[X][TOKEN_DOLLAR] = 4;  // Follow(X)

    // T -> F Y  (5)
    // FIRST(F) is { ID, NUM, ( }, so:
    parseTable[T][TOKEN_ID]     = 5;
    parseTable[T][TOKEN_NUM]    = 5;
    parseTable[T][TOKEN_LPAREN] = 5;

    // Y -> * F Y (6), / F Y (7), or ε (8)
    parseTable[Y][TOKEN_MUL]    = 6;
    parseTable[Y][TOKEN_DIV]    = 7;
    parseTable[Y][TOKEN_PLUS]   = 8;  // Follow(Y)
    parseTable[Y][TOKEN_MINUS]  = 8;  // Follow(Y)
    parseTable[Y][TOKEN_RPAREN] = 8;  // Follow(Y)
    parseTable[Y][TOKEN_DOLLAR] = 8;  // Follow(Y)

    // F -> ( E ) (9), id (10), num (11)
    parseTable[F][TOKEN_LPAREN] = 9;
    parseTable[F][TOKEN_ID]     = 10;
    parseTable[F][TOKEN_NUM]    = 11;
}

// Define all productions
// production[i][k] = symbol, 0 terminates
// Negative => terminal, Positive => non-terminal
void initProductions() {
    // 1: E -> T X
    productions[1][0] = T;
    productions[1][1] = X;
    productions[1][2] = 0;

    // 2: X -> + T X
    productions[2][0] = -TOKEN_PLUS;
    productions[2][1] = T;
    productions[2][2] = X;
    productions[2][3] = 0;

    // 3: X -> - T X
    productions[3][0] = -TOKEN_MINUS;
    productions[3][1] = T;
    productions[3][2] = X;
    productions[3][3] = 0;

    // 4: X -> ε
    productions[4][0] = 0;

    // 5: T -> F Y
    productions[5][0] = F;
    productions[5][1] = Y;
    productions[5][2] = 0;

    // 6: Y -> * F Y
    productions[6][0] = -TOKEN_MUL;
    productions[6][1] = F;
    productions[6][2] = Y;
    productions[6][3] = 0;

    // 7: Y -> / F Y
    productions[7][0] = -TOKEN_DIV;
    productions[7][1] = F;
    productions[7][2] = Y;
    productions[7][3] = 0;

    // 8: Y -> ε
    productions[8][0] = 0;

    // 9: F -> ( E )
    productions[9][0] = -TOKEN_LPAREN;
    productions[9][1] = E;
    productions[9][2] = -TOKEN_RPAREN;
    productions[9][3] = 0;

    // 10: F -> id
    productions[10][0] = -1 * (int)TOKEN_ID;
    productions[10][1] = 0;

    // 11: F -> num
    productions[11][0] = -1 * (int)TOKEN_NUM;
    productions[11][1] = 0;
}

void parse(const char* input) {
    tokenize(input);       // Convert input to token stream
    printf("\nTokens:\n");
    for (int i = 0; i < tokenCount; i++) {
        printf("%d: Type=%d, Text='%s'\n", i, tokens[i].type, tokens[i].text);
    }
    initParseTable();      // Fill parse table
    initProductions();     // Define all productions

    // Initialize stack: $ on bottom, E on top
    top = -1;
    push(-TOKEN_DOLLAR);
    push(E);

    Token lookahead = getNextToken();

    while(1) {
        int topSymbol = peek();

        if(topSymbol < 0) {
            // Top of stack is a terminal
            // Compare with lookahead token
            if(topSymbol == -(int)lookahead.type) {
                // They match! Pop and advance
                pop();
                // If matched the dollar, parsing is done
                if(lookahead.type == TOKEN_DOLLAR) {
                    printf("Parsing complete. ACCEPT.\n");
                    return;
                }
                lookahead = getNextToken();
            } else {
                // Terminal mismatch
                printf("Error: Terminal mismatch!\n");
                return;
            }
        }
        else if(topSymbol == 0) {
            // Epsilon production on top, pop it
            pop();
        }
        else {
            // Top of stack is a non-terminal
            int prodIndex = parseTable[topSymbol][lookahead.type];
            if(prodIndex == 0) {
                return;
            }
            // Pop the non-terminal
            pop();

            // Push production right-hand side in reverse
            int length = 0;
            while (productions[prodIndex][length] != 0) {
                length++;
            }
            for(int j = length - 1; j >= 0; j--) {
                push(productions[prodIndex][j]);
            }
        }
    }
}

int main() {
    char input[256];

    printf("Enter an expression: ");
    if (!fgets(input, sizeof(input), stdin)) {
        // If fgets fails or no input
        printf("No input provided.\n");
        return 1;
    }

    // Strip both newline and carriage return (handles Windows \r\n)
    input[strcspn(input, "\r\n")] = '\0';

    parse(input);
    return 0;
}
