#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROG_SIZE 1000

int run(int *prog, int a, int *out) {
    int ip = 0, b = 0, c = 0, combo;
    int out_index = 0;

    while (ip >= 0 && ip < MAX_PROG_SIZE) {
        int lit = prog[ip + 1];
        combo = (int[]){0, 1, 2, 3, a, b, c, 99999}[prog[ip + 1]];

        switch (prog[ip]) {
            case 0: a = a / (1 << combo); break;
            case 1: b = b ^ lit; break;
            case 2: b = combo % 8; break;
            case 3: ip = (a == 0) ? ip : lit - 2; break;
            case 4: b = b ^ c; break;
            case 5: out[out_index++] = combo % 8; break;
            case 6: b = a / (1 << combo); break; 
        }
        ip += 2;
    }
    return out_index;
}

int find_a(int *prog, int a, int depth, int *target, int target_length) {
    if (depth == target_length) {
        return a;
    }
    for (int i = 0; i < 8; i++) {
        int output[MAX_PROG_SIZE];
        int output_length = run(prog, a * 8 + i, output);
        if (output_length > 0 && output[0] == target[depth]) {
            int result = find_a(prog, (a * 8 + i), depth + 1, target, target_length);
            if (result) {
                return result;
            }
        }
    }
    return 0;
}

int main() {
    FILE *file = fopen("input17.txt", "r");


    int prog[MAX_PROG_SIZE];
    int a, b, c;
    int prog_size = 0;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, " \n");
        while (token) {
            prog[prog_size++] = atoi(token);
            token = strtok(NULL, " \n");
        }
    }
    fclose(file);

    a = prog[0];
    b = prog[1];
    c = prog[2];
    int target[MAX_PROG_SIZE];
    int target_length = 0;

    for (int i = prog_size - 1; i >= 0; i--) {
        target[target_length++] = prog[i];
    }

    int output[MAX_PROG_SIZE];
    int output_length = run(prog, a, output);
    for (int i = 0; i < output_length; i++) {
        printf("%d", output[i]);
        if (i < output_length - 1) {
            printf(",");
        }
    }
    printf("\n");

    int result = find_a(prog, 0, 0, target, target_length);
    printf("%d\n", result);

    return 0;
}

