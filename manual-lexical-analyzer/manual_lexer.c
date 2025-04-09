#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

// List of keywords (customize as needed)
const char* keywords[] = {"int", "float", "if", "else", "while", "return"};
int numKeywords = 6;

int isKeyword(const char* buffer) {
    for(int i = 0; i < numKeywords; i++) {
        if(strcmp(buffer, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // Expecting an input file name or read from stdin
    FILE *fp = NULL;

    if(argc > 1) {
        fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("Error: Unable to open file %s\n", argv[1]);
            return 1;
        }
    } else {
        // If no file provided, read from stdin
        fp = stdin;
        printf("Reading from stdin. Type your code and press Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) to end.\n");
    }

    char c;
    char tokenBuffer[MAX_TOKEN_LEN];
    int index = 0;

    while((c = fgetc(fp)) != EOF) {
        // Ignore whitespace, tabs, and newlines
        if(c == ' ' || c == '\t' || c == '\n') {
            continue;
        }

        // Check for identifier/keyword: starts with letter or '_'
        if(isalpha(c) || c == '_') {
            index = 0;
            tokenBuffer[index++] = c;

            // continue reading letters or digits or '_'
            while((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
                tokenBuffer[index++] = c;
            }
            tokenBuffer[index] = '\0';

            // If we have read an extra character not belonging to the token, put it back
            if(c != EOF) {
                ungetc(c, fp);
            }

            // Check if it is a keyword
            if(isKeyword(tokenBuffer)) {
                printf("[KEYWORD: %s]\n", tokenBuffer);
            } else {
                printf("[IDENTIFIER: %s]\n", tokenBuffer);
            }
        }
        // Check for number
        else if(isdigit(c)) {
            index = 0;
            tokenBuffer[index++] = c;

            // read any further digits (and possibly a decimal point if you want)
            while((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.')) {
                tokenBuffer[index++] = c;
            }
            tokenBuffer[index] = '\0';

            if(c != EOF) {
                ungetc(c, fp);
            }

            printf("[NUMBER: %s]\n", tokenBuffer);
        }
        // Otherwise, treat it as operator/punctuation
        else {
            printf("[OPERATOR/PUNCT: %c]\n", c);
        }
    }

    if(fp != stdin) {
        fclose(fp);
    }
    return 0;
}
