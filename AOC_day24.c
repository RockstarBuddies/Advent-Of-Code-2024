#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIRES 100
#define MAX_OPERATIONS 100

typedef struct {
    char name[10];
    int value;
} Wire;

typedef struct {
    char op1[10];
    char op[4];
    char op2[10];
    char res[10];
} Operation;

Wire wires[MAX_WIRES];
Operation operations[MAX_OPERATIONS];
int wire_count = 0;
int operation_count = 0;

int process(const char* op, int op1, int op2) {
    if (strcmp(op, "AND") == 0) {
        return op1 & op2;
    } else if (strcmp(op, "OR") == 0) {
        return op1 | op2;
    } else if (strcmp(op, "XOR") == 0) {
        return op1 ^ op2;
    }
    return 0;
}

int find_wire(const char* name) {
    for (int i = 0; i < wire_count; i++) {
        if (strcmp(wires[i].name, name) == 0) {
            return wires[i].value;
        }
    }
    return -1; // Not found
}

void add_wire(const char* name, int value) {
    strcpy(wires[wire_count].name, name);
    wires[wire_count].value = value;
    wire_count++;
}

int main() {
    char highest_z[10] = "z00";
    char line[100];
    FILE *file = fopen("input24.txt", "r");

    while (fgets(line, sizeof(line), file)) {
        if (strchr(line, ':')) {
            char wire[10];
            int value;
            sscanf(line, "%[^:]: %d", wire, &value);
            add_wire(wire, value);
        } else if (strchr(line, '>')) {
            char op1[10], op[4], op2[10], res[10];
            sscanf(line, "%s %s %s -> %s", op1, op, op2, res);
            strcpy(operations[operation_count].op1, op1);
            strcpy(operations[operation_count].op, op);
            strcpy(operations[operation_count].op2, op2);
            strcpy(operations[operation_count].res, res);
            operation_count++;
            if (res[0] == 'z' && atoi(res + 1) > atoi(highest_z + 1)) {
                strcpy(highest_z, res);
            }
        }
    }
    fclose(file);

    int wrong[MAX_OPERATIONS];
    int wrong_count = 0;

    for (int i = 0; i < operation_count; i++) {
        const char* op1 = operations[i].op1;
        const char* op = operations[i].op;
        const char* op2 = operations[i].op2;
        const char* res = operations[i].res;

        if (res[0] == 'z' && strcmp(op, "XOR") != 0 && strcmp(res, highest_z) != 0) {
            wrong[wrong_count++] = atoi(res + 1);
        }
        if (strcmp(op, "XOR") == 0 && res[0] != 'x' && res[0] != 'y' && res[0] != 'z' &&
            op1[0] != 'x' && op1[0] != 'y' && op1[0] != 'z' &&
            op2[0] != 'x' && op2[0] != 'y' && op2[0] != 'z') {
            wrong[wrong_count++] = atoi(res + 1);
        }
        if (strcmp(op, "AND") == 0 && strcmp(op1, "x00") != 0 && strcmp(op2, "x00") != 0) {
            for (int j = 0; j < operation_count; j++) {
                if ((strcmp(res, operations[j].op1) == 0 || strcmp(res, operations[j].op2) == 0) &&
                    strcmp(operations[j].op, "OR") != 0) {
                    wrong[wrong_count++] = atoi(res + 1);
                }
            }
        }
        if (strcmp(op, "XOR") == 0) {
            for (int j = 0; j < operation_count; j++) {
                if ((strcmp(res, operations[j].op1) == 0 || strcmp(res, operations[j].op2) == 0) &&
                    strcmp(operations[j].op, "OR") == 0) {
                    wrong[wrong_count++] = atoi(res + 1);
                }
            }
        }
    }

    while (operation_count > 0) {
        Operation current = operations[0];
        for (int i = 0; i < operation_count - 1; i++) {
            operations[i] = operations[i + 1];
        }
        operation_count--;

        int val1 = find_wire(current.op1);
        int val2 = find_wire(current.op2);
        if (val1 != -1 && val2 != -1) {
            add_wire(current.res, process(current.op, val1, val2));
        } else {
            operations[operation_count++] = current;
        }
    }

    char bits[MAX_WIRES][10];
    int bit_count = 0;
    for (int i = 0; i < wire_count; i++) {
        if (wires[i].name[0] == 'z') {
            sprintf(bits[bit_count++], "%d", wires[i].value);
        }
    }

    for (int i = bit_count - 1; i >= 0; i--) {
        printf("%s", bits[i]);
    }
    printf("\n");

    for (int i = 0; i < wrong_count; i++) {
        printf("%d", wrong[i]);
        if (i < wrong_count - 1) {
            printf(",");
        }
    }
    printf("\n");

    return 0;
}

