#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAIRS 100
#define MAX_NUMS 100
#define MAX_LENGTH 100

char pairs[MAX_PAIRS][2][MAX_LENGTH];
int pair_count = 0;

void after(const char *x, char result[MAX_NUMS][MAX_LENGTH], int *result_count) {
    *result_count = 0;
    for (int i = 0; i < pair_count; i++) {
        if (strcmp(x, pairs[i][1]) == 0) {
            strcpy(result[*result_count], pairs[i][0]);
            (*result_count)++;
        }
    }
}

int index_func(const char *x, char nums[MAX_NUMS][MAX_LENGTH], int nums_count) {
    char result[MAX_NUMS][MAX_LENGTH];
    int result_count;
    after(x, result, &result_count);
    
    int count = 0;
    for (int i = 0; i < result_count; i++) {
        for (int j = 0; j < nums_count; j++) {
            if (strcmp(result[i], nums[j]) == 0) {
                count++;
                break;
            }
        }
    }
    return count;
}

int main() {
    FILE *file = fopen("input5.txt", "r");
    

    char buffer[10000];
    fread(buffer, sizeof(char), 10000, file);
    fclose(file);

    char *order = strtok(buffer, "\n\n");
    char *updates = strtok(NULL, "\n\n");

    char *line = strtok(order, "\n");
    while (line != NULL) {
        char *pair = strtok(line, "|");
        strcpy(pairs[pair_count][0], pair);
        pair = strtok(NULL, "|");
        strcpy(pairs[pair_count][1], pair);
        pair_count++;
        line = strtok(NULL, "\n");
    }

    int was_sorted[2][MAX_NUMS];
    int was_sorted_count[2] = {0, 0};

    line = strtok(updates, "\n");
    while (line != NULL) {
        char *nums[MAX_NUMS];
        int nums_count = 0;

        char *num = strtok(line, ",");
        while (num != NULL) {
            nums[nums_count++] = num;
            num = strtok(NULL, ",");
        }

        char *nums2[MAX_NUMS];
        for (int i = 0; i < nums_count; i++) {
            nums2[i] = nums[i];
        }

        // Sort nums2 based on index_func
        for (int i = 0; i < nums_count - 1; i++) {
            for (int j = 0; j < nums_count - i - 1; j++) {
                if (index_func(nums2[j], nums, nums_count) > index_func(nums2[j + 1], nums, nums_count)) {
                    char *temp = nums2[j];
                    nums2[j] = nums2[j + 1];
                    nums2[j + 1] = temp;
                }
            }
        }

        if (memcmp(nums, nums2, nums_count * sizeof(char *)) == 0) {
            was_sorted[1][was_sorted_count[1]++] = atoi(nums[nums_count / 2]);
        } else {
            was_sorted[0][was_sorted_count[0]++] = atoi(nums2[nums_count / 2]);
        }

        line = strtok(NULL, "\n");
    }

    int sum[2] = {0, 0};
    for (int i = 0; i < was_sorted_count[0]; i++) {
        sum[0] += was_sorted[0][i];
    }
    for (int i = 0; i < was_sorted_count[1]; i++) {
        sum[1] += was_sorted[1][i];
    }

    printf("%d %d\n", sum[0], sum[1]);
    return 0;
}

