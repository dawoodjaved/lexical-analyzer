#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 10
#define MAX_RULES   20
#define MAX_RHS_LEN 10

typedef struct {
    char lhs;
    char rhs[MAX_RHS_LEN][MAX_RHS_LEN];
    int rhsCount;
} Production;


char FIRSTSets[MAX_SYMBOLS][MAX_SYMBOLS];
char FOLLOWSets[MAX_SYMBOLS][MAX_SYMBOLS];
char nonTerminals[MAX_SYMBOLS];
int ntCount = 0;

Production productions[MAX_RULES];
int prodCount = 0;

int inSet(char *set, char c) {
    for(int i = 0; set[i] != '\0'; i++) {
        if(set[i] == c) return 1;
    }
    return 0;
}

int addToSet(char *set, char c) {
    if(!inSet(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len+1] = '\0';
        return 1;
    }
    return 0;
}

int getNTIndex(char nt) {
    for(int i=0; i<ntCount; i++) {
        if(nonTerminals[i] == nt) return i;
    }
    return -1;
}

// FIRST function
int computeFIRST(char X, char *result) {
    int changed = 0;
    int index = getNTIndex(X);
    if(index < 0) {
        if(!inSet(result, X)) {
            addToSet(result, X);
            return 1;
        }
        return 0;
    }
    for(int p=0; p<productions[index].rhsCount; p++) {
        char *rhs = productions[index].rhs[p];
        for(int k=0; rhs[k] != '\0'; k++) {
            char symbol = rhs[k];
            if(symbol == '#') {
                if(!inSet(result, '#')) {
                    changed |= addToSet(result, '#');
                }
                break;
            } else {
                char temp[ MAX_SYMBOLS ];
                memset(temp, 0, sizeof(temp));
                computeFIRST(symbol, temp);
                int addedEpsilon = 0;
                for(int x=0; temp[x] != '\0'; x++) {
                    if(temp[x] == '#') {
                        addedEpsilon = 1;
                    } else {
                        changed |= addToSet(result, temp[x]);
                    }
                }
                if(!addedEpsilon) {
                    break;
                }
                if(k == (int)strlen(rhs)-1) {
                    changed |= addToSet(result, '#');
                }
            }
        }
    }
    return changed;
}

// FOLLOW function
void computeFOLLOW() {
    addToSet(FOLLOWSets[0], '$');

    int changed = 1;
    while(changed) {
        changed = 0;
        for(int i=0; i<prodCount; i++) {
            char A = productions[i].lhs;
            char* alpha;
            for(int p=0; p<productions[i].rhsCount; p++){
                alpha = productions[i].rhs[p];
                int len = strlen(alpha);
                for(int pos=0; pos<len; pos++){
                    char B = alpha[pos];
                    int BIndex = getNTIndex(B);
                    if(BIndex >= 0) {
                        int pos2;
                        for(pos2 = pos+1; pos2<len; pos2++){
                            char betaSymbol = alpha[pos2];
                            char tempFirst[MAX_SYMBOLS];
                            memset(tempFirst, 0, sizeof(tempFirst));
                            computeFIRST(betaSymbol, tempFirst);

                            for(int x=0; tempFirst[x] != '\0'; x++) {
                                if(tempFirst[x] != '#') {
                                    if(addToSet(FOLLOWSets[BIndex], tempFirst[x])) {
                                        changed = 1;
                                    }
                                }
                            }
                            if(!inSet(tempFirst, '#')){
                                break;
                            }
                        }
                        if(pos2 == len) {
                            int BAIndex = getNTIndex(A);
                            for(int f=0; FOLLOWSets[BAIndex][f] != '\0'; f++){
                                if(addToSet(FOLLOWSets[BIndex], FOLLOWSets[BAIndex][f])) {
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    
    // Non-terminals:
    nonTerminals[0] = 'S';
    nonTerminals[1] = 'A';
    nonTerminals[2] = 'B';
    nonTerminals[3] = 'C';
    ntCount = 4;

    // Production for S
    productions[0].lhs = 'S';
    strcpy(productions[0].rhs[0], "ABC");
    productions[0].rhsCount = 1;
    prodCount++;

    // Production for A
    productions[1].lhs = 'A';
    strcpy(productions[1].rhs[0], "a");
    strcpy(productions[1].rhs[1], "#");
    productions[1].rhsCount = 2;
    prodCount++;

    // Production for B
    productions[2].lhs = 'B';
    strcpy(productions[2].rhs[0], "b");
    productions[2].rhsCount = 1;
    prodCount++;

    // Production for C
    productions[3].lhs = 'C';
    strcpy(productions[3].rhs[0], "c");
    strcpy(productions[3].rhs[1], "#");
    productions[3].rhsCount = 2;
    prodCount++;

    // Initialize FIRST/FOLLOW sets
    for(int i=0; i<ntCount; i++){
        FIRSTSets[i][0] = '\0';
        FOLLOWSets[i][0] = '\0';
    }

    // Compute FIRST sets for each non-terminal
    int updated = 1;
    while(updated) {
        updated = 0;
        for(int i=0; i<ntCount; i++){
            char tmp[MAX_SYMBOLS];
            memset(tmp, 0, sizeof(tmp));
            // compute FIRST(nonTerminals[i]) into tmp
            computeFIRST(nonTerminals[i], tmp);
            // union tmp into FIRSTSets[i]
            for(int t=0; tmp[t] != '\0'; t++){
                if(addToSet(FIRSTSets[i], tmp[t])) {
                    updated = 1;
                }
            }
        }
    }

    // Compute FOLLOW sets
    computeFOLLOW();

    // Print results
    printf("FIRST sets:\n");
    for(int i=0; i<ntCount; i++){
        printf("FIRST(%c) = { ", nonTerminals[i]);
        for(int j=0; FIRSTSets[i][j] != '\0'; j++){
            printf("%c ", FIRSTSets[i][j]);
        }
        printf("}\n");
    }
    printf("\nFOLLOW sets:\n");
    for(int i=0; i<ntCount; i++){
        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for(int j=0; FOLLOWSets[i][j] != '\0'; j++){
            printf("%c ", FOLLOWSets[i][j]);
        }
        printf("}\n");
    }
    return 0;
}
