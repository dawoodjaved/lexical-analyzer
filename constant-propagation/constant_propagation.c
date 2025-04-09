#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STMTS 50

typedef struct {
    char var[10];
    char expr[50];
} Stmt;

typedef struct {
    char var[10];
    int value;
    int isKnown;
} ConstMap;

Stmt stmts[MAX_STMTS];
int stmtCount = 0;
ConstMap constMap[50];
int constCount = 0;

int lookupConst(char *var, int *val) {
    for (int i = 0; i < constCount; i++) {
        if (strcmp(constMap[i].var, var) == 0 && constMap[i].isKnown) {
            *val = constMap[i].value;
            return 1;
        }
    }
    return 0;
}

void setConst(char *var, int val) {
    for (int i = 0; i < constCount; i++) {
        if (strcmp(constMap[i].var, var) == 0) {
            constMap[i].value = val;
            constMap[i].isKnown = 1;
            return;
        }
    }
    strcpy(constMap[constCount].var, var);
    constMap[constCount].value = val;
    constMap[constCount].isKnown = 1;
    constCount++;
}

int isNumber(char *str) {
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}

void parseStmt(char *line) {
    char var[10], eq[3], expr[50];
    sscanf(line, "%s %s %[^\n]", var, eq, expr);
    strcpy(stmts[stmtCount].var, var);
    strcpy(stmts[stmtCount].expr, expr);
    stmtCount++;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");
    if (!fin) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), fin)) {
        if (line[0] == '\n') break;
        line[strcspn(line, "\n")] = '\0';
        parseStmt(line);
    }
    fclose(fin);

    for (int i = 0; i < stmtCount; i++) {
        char exprCopy[50];
        strcpy(exprCopy, stmts[i].expr);
        char *t1 = strtok(exprCopy, " ");
        char *t2 = strtok(NULL, " ");
        char *t3 = strtok(NULL, " ");

        if (!t2) {
            if (isNumber(t1)) {
                int val = atoi(t1);
                setConst(stmts[i].var, val);
                printf("%s = %d\n", stmts[i].var, val);
            } else {
                int val;
                if (lookupConst(t1, &val)) {
                    setConst(stmts[i].var, val);
                    printf("%s = %d\n", stmts[i].var, val);
                } else {
                    printf("%s = %s\n", stmts[i].var, t1);
                }
            }
        } else if (strcmp(t2, "+") == 0) {
            int lval, rval;
            int lknown = isNumber(t1) ? 1 : lookupConst(t1, &lval);
            int rknown = isNumber(t3) ? 1 : lookupConst(t3, &rval);
            if (isNumber(t1)) lval = atoi(t1);
            if (isNumber(t3)) rval = atoi(t3);

            if (lknown && rknown) {
                int sum = lval + rval;
                setConst(stmts[i].var, sum);
                printf("%s = %d\n", stmts[i].var, sum);
            } else if (lknown) {
                printf("%s = %d + %s\n", stmts[i].var, lval, t3);
            } else if (rknown) {
                printf("%s = %s + %d\n", stmts[i].var, t1, rval);
            } else {
                printf("%s = %s + %s\n", stmts[i].var, t1, t3);
            }
        } else {
            printf("%s = %s\n", stmts[i].var, stmts[i].expr);
        }
    }

    return 0;
}
