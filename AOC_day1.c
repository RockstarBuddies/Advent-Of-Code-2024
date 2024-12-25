#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LENGTH 100

int left[MAX_LINES];
int right[MAX_LINES];
int count_left = 0;
int count_right = 0;

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main() {
    FILE *input = fopen("input.txt", "r");
    

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        char *token = strtok(line, "   ");
        if (token != NULL) {
            left[count_left++] = atoi(token);
        }
        token = strtok(NULL, "   ");
        if (token != NULL) {
            right[count_right++] = atoi(token);
        }
    }
    fclose(input);

    qsort(left, count_left, sizeof(int), compare);
    qsort(right, count_right, sizeof(int), compare);

    int distance = 0;
    for (int i = 0; i < count_left; i++) {
        distance += abs(left[i] - right[i]);
    }
    printf("%d\n", distance);

    int rd[MAX_LINES] = {0};
    for (int i = 0; i < count_right; i++) {
        rd[right[i]]++;
    }

    int similarity_score = 0;
    for (int i = 0; i < count_left; i++) {
        similarity_score += left[i] * rd[left[i]];
    }
    printf("%d\n", similarity_score);

    return 0;
}

    