#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define INF 9999

typedef struct {
    int x, y;
} Point;

Point bytes_in[MAX_SIZE];
int sz;

int adjs(Point xy, Point *neighbors) {
    neighbors[0] = (Point){xy.x + 1, xy.y};
    neighbors[1] = (Point){xy.x - 1, xy.y};
    neighbors[2] = (Point){xy.x, xy.y + 1};
    neighbors[3] = (Point){xy.x, xy.y - 1};
    return 4;
}

int is_ok(Point *obstacles, int obstacle_count, Point xy) {
    for (int i = 0; i < obstacle_count; i++) {
        if (obstacles[i].x == xy.x && obstacles[i].y == xy.y) {
            return 0;
        }
    }
    return xy.x >= 0 && xy.x <= sz && xy.y >= 0 && xy.y <= sz;
}

int bfs(Point *obstacles, int obstacle_count, int *dist, Point *layers, int layer_count) {
    int edge_count = 0;
    Point edge[MAX_SIZE];

    for (int i = 0; i < layer_count; i++) {
        for (int j = 0; j < 4; j++) {
            Point neighbors[4];
            int n_count = adjs(layers[i], neighbors);
            for (int k = 0; k < n_count; k++) {
                if (is_ok(obstacles, obstacle_count, neighbors[k]) && dist[neighbors[k].x * (sz + 1) + neighbors[k].y] == INF) {
                    edge[edge_count++] = neighbors[k];
                }
            }
        }
    }

    for (int i = 0; i < edge_count; i++) {
        dist[edge[i].x * (sz + 1) + edge[i].y] = layer_count;
        layers[layer_count] = edge[i];
    }

    return edge_count;
}

int distance(Point *obstacles, int obstacle_count, Point start, Point end) {
    int *dist = (int *)malloc((sz + 1) * (sz + 1) * sizeof(int));
    

    for (int i = 0; i < (sz + 1) * (sz + 1); i++) {
        dist[i] = INF;
    }
    dist[start.x * (sz + 1) + start.y] = 0;

    Point layers[MAX_SIZE];
    layers[0] = start;

    int layer_count = 1;
    while (bfs(obstacles, obstacle_count, dist, layers, layer_count) > 0) {
        layer_count++;
    }

    int result = dist[end.x * (sz + 1) + end.y];
    free(dist);
    return result;
}

int main() {
    FILE *file = fopen("input18.txt", "r");


    int count = 0;
    while (fscanf(file, "%d,%d\n", &bytes_in[count].x, &bytes_in[count].y) != EOF) {
        count++;
    }
    fclose(file);

    sz = 0;
    for (int i = 0; i < count; i++) {
        if (bytes_in[i].x > sz) sz = bytes_in[i].x;
        if (bytes_in[i].y > sz) sz = bytes_in[i].y;
    }

    printf("%d\n", distance(bytes_in, count, (Point){0, 0}, (Point){sz, sz}));

    for (int i = 0; i < count; i++) {
        if (distance(bytes_in, i, (Point){0, 0}, (Point){sz, sz}) == INF) {
            printf("%d\n", bytes_in[i].x);
            break;
        }
    }

    return 0;
}