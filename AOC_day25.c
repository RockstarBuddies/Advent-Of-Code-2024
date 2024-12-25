#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_COLUMNS 100

int main() {
    FILE *file = fopen("input25.txt", "r");


    char buffer[1024];
    char *keys[MAX_LINES][MAX_COLUMNS];
    char *locks[MAX_LINES][MAX_COLUMNS];
    int keys_count = 0, locks_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strcmp(buffer, "\n") == 0) {
            continue;
        }

        char *line[MAX_COLUMNS];
        int line_length = 0;
        char *token = strtok(buffer, " \n");
        while (token != NULL) {
            line[line_length++] = token;
            token = strtok(NULL, " \n");
        }

        int count_hash = 0;
        for (int i = 0; i < line_length; i++) {
            if (line[i][0] == '#') {
                count_hash++;
            }
        }

        if (line[0][0] == '#') {
            for (int i = 0; i < line_length; i++) {
                locks[locks_count][i] = (char *)malloc(sizeof(char) * (count_hash + 1));
                strcpy(locks[locks_count][i], line[i]);
            }
            locks_count++;
        } else {
            for (int i = 0; i < line_length; i++) {
                keys[keys_count][i] = (char *)malloc(sizeof(char) * (count_hash + 1));
                strcpy(keys[keys_count][i], line[i]);
            }
            keys_count++;
        }
    }
    fclose(file);

    int total = 0;
    for (int i = 0; i < locks_count; i++) {
        for (int j = 0; j < keys_count; j++) {
            int lock_sum = 0, key_sum = 0;
            for (int k = 0; k < MAX_COLUMNS; k++) {
                if (locks[i][k] != NULL) {
                    lock_sum += (locks[i][k][0] == '#') ? 1 : 0;
                }
                if (keys[j][k] != NULL) {
                    key_sum += (keys[j][k][0] == '#') ? 1 : 0;
                }
            }
            if (lock_sum + key_sum <= 5) {
                total++;
            }
        }
    }

    printf("%d\n", total);
    return 0;
}

