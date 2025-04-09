#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    char line[256];
    char loopHeader[256];
    char loopBody[1024] = "";
    int stage = 0;

    while (fgets(line, sizeof(line), fin)) {
        if (strstr(line, "}")) break;
        if (stage == 0) {
            strcpy(loopHeader, line);
            stage = 1;
        } else {
            strcat(loopBody, line);
        }
    }
    fclose(fin);

    // Unroll the loop header
    char unrolledHeader[256];
    strcpy(unrolledHeader, loopHeader);
    char *inc = strstr(unrolledHeader, "i++");
    if (inc) {
        inc[1] = '+';
        inc[2] = '=';
        inc[3] = '2';
    }

    printf("--- Unrolled Code ---\n");
    printf("%s", unrolledHeader);
    printf("{\n   // Original body\n   %s", loopBody);

    printf("   // Unrolled body\n");
    printf("   if(i+1 < n) {\n");

    // Replace all "i]" with "i+1]"
    char copyBody[1024];
    strcpy(copyBody, loopBody);
    for (char *p = strstr(copyBody, "i]"); p; p = strstr(p + 1, "i]")) {
        memmove(p + 2, p + 1, strlen(p + 1) + 1);
        p[1] = '+';
        p[2] = '1';
    }
    printf("   %s", copyBody);
    printf("   }\n}\n");

    return 0;
}
