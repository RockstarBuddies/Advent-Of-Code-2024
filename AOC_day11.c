#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STONES 1000

void blink(int **arrangement, int *arr_size) {
    for (int i = 0; i < *arr_size; i++) {
        if (arrangement[i] != NULL) {
            int *newgroup = (int *)malloc(MAX_STONES * sizeof(int));
            int new_size = 0;
            for (int j = 0; j < *arrangement[i]; j++) {
                if (arrangement[i][j] == 0) {
                    newgroup[new_size++] = 1;
                } else {
                    char str[20];
                    sprintf(str, "%d", arrangement[i][j]);
                    int len = strlen(str);
                    if (len % 2 == 0) {
                        int subgroup1 = atoi(strndup(str, len / 2));
                        int subgroup2 = atoi(strndup(str + len / 2, len / 2));
                        newgroup[new_size++] = subgroup1;
                        newgroup[new_size++] = subgroup2;
                    } else {
                        newgroup[new_size++] = arrangement[i][j] * 2024;
                    }
                }
            }
            free(arrangement[i]);
            arrangement[i] = (int *)malloc(new_size * sizeof(int));
            memcpy(arrangement[i], newgroup, new_size * sizeof(int));
            free(newgroup);
        } else {
            if (arrangement[i][0] == 0) {
                arrangement[i][0] = 1;
            } else {
                char str[20];
                sprintf(str, "%d", arrangement[i][0]);
                int len = strlen(str);
                if (len % 2 == 0) {
                    int subgroup1 = atoi(strndup(str, len / 2));
                    int subgroup2 = atoi(strndup(str + len / 2, len / 2));
                    arrangement[i] = (int *)malloc(2 * sizeof(int));
                    arrangement[i][0] = subgroup1;
                    arrangement[i][1] = subgroup2;
                } else {
                    arrangement[i][0] *= 2024;
                }
            }
        }
    }
}

int blink_rec(int stone) {
    if (stone == 0) {
        return 1;
    } else {
        char str[20];
        sprintf(str, "%d", stone);
        int len = strlen(str);
        if (len % 2 == 0) {
            return (atoi(strndup(str, len / 2)), atoi(strndup(str + len / 2, len / 2)));
        }
    }
    return stone * 2024;
}

int split_count(int stone, int blinks, int memo_dict[MAX_STONES][MAX_STONES]) {
    if (blinks == 0) {
        memo_dict[stone][blinks] = 1;
        return 1;
    }
    if (memo_dict[stone][blinks] != -1) {
        return memo_dict[stone][blinks];
    }

    int blink_rez = blink_rec(stone);
    int splits = 0;
    if (blink_rez != stone * 2024) {
        for (int substone = 0; substone < blink_rez; substone++) {
            splits += split_count(substone, blinks - 1, memo_dict);
        }
        memo_dict[stone][blinks] = splits;
        return splits;
    } else {
        splits = split_count(blink_rez, blinks - 1, memo_dict);
        memo_dict[stone][blinks] = splits;
        return splits;
    }
}

int main() {
    FILE *file = fopen("input11.txt", "r");
    int arrangement[MAX_STONES][MAX_STONES];
    int arr_size = 0;

    while (fscanf(file, "%d", &arrangement[arr_size][0]) != EOF) {
        arr_size++;
    }
    fclose(file);

    int arr_copy[MAX_STONES][MAX_STONES];
    memcpy(arr_copy, arrangement, sizeof(arrangement));
    int prev_len = 0;

    for (int i = 0; i < 25; i++) {
        blink(arrangement, &arr_size);
    }

    int num_stones = 0;
    for (int i = 0; i < arr_size; i++) {
        num_stones += sizeof(arrangement[i]) / sizeof(arrangement[i][0]);
    }
    printf("%d\n", num_stones);

    int memo_dict[MAX_STONES][MAX_STONES];
    memset(memo_dict, -1, sizeof(memo_dict));

    int total_stones = 0;
    for (int i = 0; i < arr_size; i++) {
        int rez = split_count(arr_copy[i][0], 75, memo_dict);
        total_stones += rez;
    }
    printf("%d\n", total_stones);

    return 0;
}

