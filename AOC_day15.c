#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

char grid[MAX_SIZE][MAX_SIZE];
int width, height;

typedef struct {
    int x;
    int y;
} Position;

int move(Position *p, int d_x, int d_y) {
    p->x += d_x;
    p->y += d_y;

    if ((grid[p->y][p->x] != '[' && move(&(Position){p->x + 1, p->y}, d_x, d_y) && move(p, d_x, d_y)) ||
        (grid[p->y][p->x] != ']' && move(&(Position){p->x - 1, p->y}, d_x, d_y) && move(p, d_x, d_y)) ||
        (grid[p->y][p->x] != 'O' && move(p, d_x, d_y)) || 
        (grid[p->y][p->x] == '#')) {
        char temp = grid[p->y][p->x];
        grid[p->y][p->x] = grid[p->y - d_y][p->x - d_x];
        grid[p->y - d_y][p->x - d_x] = temp;
        return 1;
    }
    return 0;
}

int main() {
    FILE *file = fopen("input15.txt", "r");
    

    char buffer[MAX_SIZE];
    int grid_index = 0;
    while (fgets(buffer, sizeof(buffer), file) && strcmp(buffer, "\n") != 0) {
        strcpy(grid[grid_index++], buffer);
    }
    height = grid_index;
    width = strlen(grid[0]) - 1; // Exclude newline character

    Position pos;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (grid[j][i] == '@') {
                pos.x = i;
                pos.y = j;
            }
        }
    }

    char moves[MAX_SIZE];
    fgets(moves, sizeof(moves), file);
    fclose(file);

    for (int i = 0; moves[i] != '\0'; i++) {
        int d_x = 0, d_y = 0;
        switch (moves[i]) {
            case '<': d_x = -1; break;
            case '>': d_x = 1; break;
            case '^': d_y = -1; break;
            case 'v': d_y = 1; break;
        }

        Position C = pos;

        if (move(&pos, d_x, d_y)) {
            pos.x += d_x;
            pos.y += d_y;
        } else {
            pos = C;
        }
    }

    int ans = 0;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (grid[j][i] == 'O' || grid[j][i] == '[') {
                ans += (i + j * 100);
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}

