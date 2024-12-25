#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_H 100
#define MAX_W 100
#define TARGET "XMAS"
#define TARGET_LEN 4

char grid[MAX_H][MAX_W];
int H, W;

int main() {
    FILE *file = fopen("input4.txt", "r");
    
    char line[MAX_W];
    H = 0;
    while (fgets(line, sizeof(line), file)) {
        strcpy(grid[H], line);
        H++;
    }
    fclose(file);
    
    W = strlen(grid[0]) - 1; // Exclude newline character

    // Part 1 - Find anything that says 'XMAS'
    int DELTAS[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int count = 0;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            for (int d = 0; d < 8; d++) {
                int dy = DELTAS[d][0];
                int dx = DELTAS[d][1];
                char candidate[TARGET_LEN + 1];
                candidate[TARGET_LEN] = '\0';
                for (int i = 0; i < TARGET_LEN; i++) {
                    int ny = y + dy * i;
                    int nx = x + dx * i;
                    if (ny >= 0 && ny < H && nx >= 0 && nx < W) {
                        candidate[i] = grid[ny][nx];
                    } else {
                        candidate[i] = '\0';
                        break;
                    }
                }
                count += strcmp(candidate, TARGET) == 0;
            }
        }
    }
    printf("Part 1: %d\n", count);

    // Part 2 - Find an MAS 'X'...
    count = 0;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (grid[y][x] == 'A') {
                char lr[3] = {0}, rl[3] = {0};
                lr[0] = grid[y-1][x-1];
                lr[1] = grid[y+1][x+1];
                rl[0] = grid[y-1][x+1];
                rl[1] = grid[y+1][x-1];
                if ((strcmp(lr, "MS") == 0 && strcmp(rl, "SM") == 0) || 
                    (strcmp(lr, "SM") == 0 && strcmp(rl, "MS") == 0)) {
                    count++;
                }
            }
        }
    }
    printf("Part 2: %d\n", count);

    return 0;
}

