#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_PQ_SIZE 10000

typedef struct {
    int row;
    int col;
    char direction;
} State;

typedef struct {
    int dist;
    State state;
} PQElement;

typedef struct {
    PQElement elements[MAX_PQ_SIZE];
    int size;
} PriorityQueue;

void init_pq(PriorityQueue *pq) {
    pq->size = 0;
}

int is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

void push(PriorityQueue *pq, int dist, State state) {
    pq->elements[pq->size].dist = dist;
    pq->elements[pq->size].state = state;
    pq->size++;
    // Simple bubble up for priority queue
    for (int i = pq->size - 1; i > 0; i--) {
        if (pq->elements[i].dist < pq->elements[(i - 1) / 2].dist) {
            PQElement temp = pq->elements[i];
            pq->elements[i] = pq->elements[(i - 1) / 2];
            pq->elements[(i - 1) / 2] = temp;
        } else {
            break;
        }
    }
}

PQElement pop(PriorityQueue *pq) {
    PQElement result = pq->elements[0];
    pq->elements[0] = pq->elements[--pq->size];
    // Simple bubble down for priority queue
    int i = 0;
    while (i * 2 + 1 < pq->size) {
        int child = i * 2 + 1;
        if (child + 1 < pq->size && pq->elements[child + 1].dist < pq->elements[child].dist) {
            child++;
        }
        if (pq->elements[i].dist > pq->elements[child].dist) {
            PQElement temp = pq->elements[i];
            pq->elements[i] = pq->elements[child];
            pq->elements[child] = temp;
            i = child;
        } else {
            break;
        }
    }
    return result;
}

char grid[MAX_ROWS][MAX_COLS];
int rows, cols;

void parse(char *lines[], int line_count, State *start, State *end) {
    rows = line_count;
    for (int i = 0; i < line_count; i++) {
        strcpy(grid[i], lines[i]);
        for (int j = 0; j < strlen(lines[i]); j++) {
            if (lines[i][j] == 'S') {
                start->row = i;
                start->col = j;
                start->direction = 'E'; // Initial direction
            } else if (lines[i][j] == 'E') {
                end->row = i;
                end->col = j;
            }
        }
    }
}

int dijkstra(State starts[], int start_count, int dist[MAX_ROWS][MAX_COLS][4]) {
    PriorityQueue pq;
    init_pq(&pq);
    int delta[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}}; // E, W, N, S
    char directions[4] = {'E', 'W', 'N', 'S'};

    for (int i = 0; i < start_count; i++) {
        State s = starts[i];
        dist[s.row][s.col][0] = 0; // Starting direction
        push(&pq, 0, s);
    }

    while (!is_empty(&pq)) {
        PQElement current = pop(&pq);
        int d = current.dist;
        State state = current.state;

        if (dist[state.row][state.col][state.direction - 'E'] < d) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            if (directions[i] != state.direction) {
                State next_state = {state.row, state.col, directions[i]};
                if (dist[next_state.row][next_state.col][next_state.direction - 'E'] > d + 1000) {
                    dist[next_state.row][next_state.col][next_state.direction - 'E'] = d + 1000;
                    push(&pq, d + 1000, next_state);
                }
            }
        }

        int dr = delta[state.direction - 'E'][0];
        int dc = delta[state.direction - 'E'][1];
        int next_row = state.row + dr;
        int next_col = state.col + dc;

        if (next_row >= 0 && next_row < rows && next_col >= 0 && next_col < cols && grid[next_row][next_col] != '#') {
            State next_state = {next_row, next_col, state.direction};
            if (dist[next_row][next_col][state.direction - 'E'] > d + 1) {
                dist[next_row][next_col][state.direction - 'E'] = d + 1;
                push(&pq, d + 1, next_state);
            }
        }
    }

    return 0;
}

int part1(State start, State end) {
    int dist[MAX_ROWS][MAX_COLS][4];
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            for (int k = 0; k < 4; k++) {
                dist[i][j][k] = INT_MAX;
            }
        }
    }

    State starts[1] = {start};
    dijkstra(starts, 1, dist);
    int best = INT_MAX;

    for (int i = 0; i < 4; i++) {
        if (dist[end.row][end.col][i] < best) {
            best = dist[end.row][end.col][i];
        }
    }
    return best;
}

int part2(State start, State end) {
    int dist_from_start[MAX_ROWS][MAX_COLS][4];
    int dist_from_end[MAX_ROWS][MAX_COLS][4];
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            for (int k = 0; k < 4; k++) {
                dist_from_start[i][j][k] = INT_MAX;
                dist_from_end[i][j][k] = INT_MAX;
            }
        }
    }

    State starts[1] = {start};
    dijkstra(starts, 1, dist_from_start);

    State ends[4] = {{end.row, end.col, 'E'}, {end.row, end.col, 'W'}, {end.row, end.col, 'N'}, {end.row, end.col, 'S'}};
    dijkstra(ends, 4, dist_from_end);

    int optimal = part1(start, end);
    char flip[4] = {'W', 'E', 'S', 'N'};
    int result_count = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            for (int dir = 0; dir < 4; dir++) {
                State state_from_start = {row, col, 'E' + dir};
                State state_from_end = {row, col, flip[dir]};
                if (dist_from_start[state_from_start.row][state_from_start.col][state_from_start.direction - 'E'] < INT_MAX &&
                    dist_from_end[state_from_end.row][state_from_end.col][state_from_end.direction - 'E'] < INT_MAX) {
                    if (dist_from_start[state_from_start.row][state_from_start.col][state_from_start.direction - 'E'] +
                        dist_from_end[state_from_end.row][state_from_end.col][state_from_end.direction - 'E'] == optimal) {
                        result_count++;
                    }
                }
            }
        }
    }
    return result_count;
}

int main() {
    FILE *file = fopen("input16.txt", "r");

    char *lines[MAX_ROWS];
    char buffer[256];
    int line_count = 0;

    while (fgets(buffer, sizeof(buffer), file) && line_count < MAX_ROWS) {
        lines[line_count] = strdup(buffer);
        line_count++;
    }
    fclose(file);

    State start, end;
    parse(lines, line_count, &start, &end);

    printf("%d\n", part1(start, end));
    printf("%d\n", part2(start, end));

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    return 0;
}

