#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 100
#define MAX_NUMBERS 10

int read_lines(const char *filename, int levels[MAX_LINES][MAX_NUMBERS], int *line_count) {
    FILE *f = fopen(filename, "r");
    

    char buffer[256];
    *line_count = 0;

    while (fgets(buffer, sizeof(buffer), f) && *line_count < MAX_LINES) {
        char *token = strtok(buffer, "\n");
        if (token) {
            int count = 0;
            char *num = strtok(token, " ");
            while (num != NULL && count < MAX_NUMBERS) {
                levels[*line_count][count++] = atoi(num);
                num = strtok(NULL, " ");
            }
            (*line_count)++;
        }
    }
    fclose(f);
    return 0;
}

bool safe1(int levels[], int size) {
    for (int i = 1; i < size; i++) {
        int n1 = levels[i - 1];
        int n2 = levels[i];
        if (!(1 <= abs(n1 - n2) && abs(n1 - n2) <= 3)) {
            return false;
        }
    }
    bool ascending = true, descending = true;
    for (int i = 1; i < size; i++) {
        if (levels[i] < levels[i - 1]) ascending = false;
        if (levels[i] > levels[i - 1]) descending = false;
    }
    return ascending || descending;
}

bool safe2(int levels[], int size) {
    for (int i = 0; i < size; i++) {
        int temp[MAX_NUMBERS];
        int temp_size = 0;
        for (int j = 0; j < size; j++) {
            if (j != i) {
                temp[temp_size++] = levels[j];
            }
        }
        if (safe1(temp, temp_size)) {
            return true;
        }
    }
    return false;
}

int main() {
    int levels[MAX_LINES][MAX_NUMBERS];
    int line_count;

    if (read_lines("input2.txt", levels, &line_count) != 0) {
        fprintf(stderr, "Error reading file\n");
        return 1;
    }

    // Part 1
    int part1_sum = 0;
    for (int i = 0; i < line_count; i++) {
        part1_sum += safe1(levels[i], MAX_NUMBERS);
    }
    printf("%d\n", part1_sum);

    // Part 2
    int part2_sum = 0;
    for (int i = 0; i < line_count; i++) {
        part2_sum += safe2(levels[i], MAX_NUMBERS);
    }
    printf("%d\n", part2_sum);

    return 0;
}

