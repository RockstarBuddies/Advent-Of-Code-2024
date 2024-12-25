#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point points[MAX_SIZE];
    int size;
} PointList;

typedef struct {
    Point point;
    int distance;
} DistanceEntry;

typedef struct {
    DistanceEntry entries[MAX_SIZE];
    int size;
} DistanceMap;

PointList todo;
DistanceMap dist;
char grid[MAX_SIZE][MAX_SIZE];
int rows = 0, cols = 0;

int is_valid(Point p) {
    return p.x >= 0 && p.x < rows && p.y >= 0 && p.y < cols && grid[p.x][p.y] != '#';
}

int main() {
    FILE *file = fopen("input20.txt", "r");
    
    while (fgets(grid[rows], sizeof(grid[0]), file)) {
        cols = strlen(grid[rows]) - 1; 
        rows++;
    }
    fclose(file);

    // Initialize the starting point
    Point start;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'S') {
                start.x = i;
                start.y = j;
                break;
            }
        }
    }

    // Initialize distance map
    dist.size = 0;
    dist.entries[dist.size++] = (DistanceEntry){start, 0};
    todo.size = 0;
    todo.points[todo.size++] = start;

    // BFS to calculate distances
    while (todo.size > 0) {
        Point pos = todo.points[--todo.size];
        Point directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int d = 0; d < 4; d++) {
            Point new_pos = {pos.x + directions[d].x, pos.y + directions[d].y};
            if (is_valid(new_pos)) {
                int found = 0;
                for (int k = 0; k < dist.size; k++) {
                    if (dist.entries[k].point.x == new_pos.x && dist.entries[k].point.y == new_pos.y) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    dist.entries[dist.size++] = (DistanceEntry){new_pos, dist.entries[dist.size - 1].distance + 1};
                    todo.points[todo.size++] = new_pos;
                }
            }
        }
    }

    int a = 0, b = 0;

    // Calculate a and b
    for (int i = 0; i < dist.size; i++) {
        for (int j = i + 1; j < dist.size; j++) {
            Point p = dist.entries[i].point;
            Point q = dist.entries[j].point;
            int d = abs(p.x - q.x) + abs(p.y - q.y);
            int j_i_d = dist.entries[j].distance - dist.entries[i].distance - d;

            if (d == 2 && j_i_d >= 100) a++;
            if (d < 21 && j_i_d >= 100) b++;
        }
    }

    printf("%d\n", a);
    printf("%d\n", b);

    return 0;
}

