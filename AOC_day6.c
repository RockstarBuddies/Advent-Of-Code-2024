#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

char kaart[MAX_SIZE][MAX_SIZE];
int seen[MAX_SIZE][MAX_SIZE][4][4]; // To track seen states
int rows, cols;

typedef struct {
    int r, c, dr, dc;
} State;

State queue[MAX_SIZE * MAX_SIZE];
int front, rear;

void enqueue(State s) {
    queue[rear++] = s;
}

State dequeue() {
    return queue[front++];
}

int is_empty() {
    return front == rear;
}

int path(int p1, int start_r, int start_c) {
    front = rear = 0;
    enqueue((State){start_r, start_c, -1, 0});
    int dr = -1, dc = 0;
    int ds[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int count = 0;

    while (!is_empty()) {
        State current = dequeue();
        int r = current.r, c = current.c, d_r = current.dr, d_c = current.dc;

        if (seen[r][c][d_r + 1][d_c + 1]) return 1;
        if (p1) seen[r][c][0][0] = 1;
        else seen[r][c][d_r + 1][d_c + 1] = 1;

        int nr = r + d_r, nc = c + d_c;
        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
            if (p1) {
                printf("%d\n", count);
                return count;
            } else return 0;
        } else if (kaart[nr][nc] == '#') {
            int temp_dr = d_r, temp_dc = d_c;
            d_r = ds[(d_r + 1) % 4][0];
            d_c = ds[(d_c + 1) % 4][1];
            enqueue((State){r, c, d_r, d_c});
        } else {
            enqueue((State){nr, nc, d_r, d_c});
        }
    }
    return 0;
}

int main() {
    FILE *file = fopen("input6.txt", "r");
    char line[MAX_SIZE];
    rows = 0;

    while (fgets(line, sizeof(line), file)) {
        strcpy(kaart[rows++], line);
    }
    fclose(file);
    cols = strlen(kaart[0]) - 1; // Exclude newline character

    int start_r, start_c;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (kaart[r][c] == '^') {
                start_r = r;
                start_c = c;
            }
        }
    }

    memset(seen, 0, sizeof(seen));
    seen[start_r][start_c][0][0] = 1;

    int visited = path(1, start_r, start_c);
    int p2 = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (kaart[r][c] == '^' || kaart[r][c] == '#') continue;
            kaart[r][c] = '#';
            memset(seen, 0, sizeof(seen));
            seen[start_r][start_c][0][0] = 1;
            p2 += path(0, start_r, start_c);
            kaart[r][c] = '.';
        }
    }

    printf("%d\n", p2);
    return 0;
}