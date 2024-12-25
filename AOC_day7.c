#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NUMBERS 100
#define MAX_LINE_LENGTH 256

void pr(const char *s) {
    printf("%s\n", s);
    // Note: Copying to clipboard is not directly supported in C without platform-specific libraries.
}

bool is_valid(int target, int *ns, int size, bool p2) {
    if (size == 1) {
        return ns[0] == target;
    }
    for (int i = 0; i < size - 1; i++) {
        int new_ns[MAX_NUMBERS];
        int new_size = 0;

        // Addition
        new_ns[new_size++] = ns[i] + ns[i + 1];
        for (int j = 0; j < size; j++) {
            if (j != i && j != i + 1) {
                new_ns[new_size++] = ns[j];
            }
        }
        if (is_valid(target, new_ns, new_size, p2)) {
            return true;
        }

        // Multiplication
        new_ns[0] = ns[i] * ns[i + 1];
        new_size = 1;
        for (int j = 0; j < size; j++) {
            if (j != i && j != i + 1) {
                new_ns[new_size++] = ns[j];
            }
        }
        if (is_valid(target, new_ns, new_size, p2)) {
            return true;
        }

        // Concatenation
        if (p2) {
            char concat[20];
            sprintf(concat, "%d%d", ns[i], ns[i + 1]);
            int concatenated_value = atoi(concat);
            new_ns[0] = concatenated_value;
            new_size = 1;
            for (int j = 0; j < size; j++) {
                if (j != i && j != i + 1) {
                    new_ns[new_size++] = ns[j];
                }
            }
            if (is_valid(target, new_ns, new_size, p2)) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    int p1 = 0, p2 = 0;
    char infile[MAX_LINE_LENGTH] = "input7.txt";
    if (argc >= 2) {
        strcpy(infile, argv[1]);
    }

    FILE *file = fopen(infile, "r");
    

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int target;
        int ns[MAX_NUMBERS];
        int size = 0;

        char *token = strtok(line, ":");
        target = atoi(token);
        token = strtok(NULL, ":");
        char *num = strtok(token, " ");
        while (num != NULL) {
            ns[size++] = atoi(num);
            num = strtok(NULL, " ");
        }

        if (is_valid(target, ns, size, false)) {
            p1 += target;
        }
        if (is_valid(target, ns, size, true)) {
            p2 += target;
        }
    }

    fclose(file);

    char result[20];
    sprintf(result, "%d", p1);
    pr(result);
    sprintf(result, "%d", p2);
    pr(result);

    return 0;
}
