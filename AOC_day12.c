#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000
#define MAX_SETS 1000

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point points[MAX_SIZE];
    int size;
} Set;

Set sets[MAX_SETS];
int set_count = 0;

int grid[MAX_SIZE][MAX_SIZE];
int grid_size_x = 0;
int grid_size_y = 0;

void add_to_set(Set *set, Point p) {
    set->points[set->size++] = p;
}

int in_set(Set *set, Point p) {
    for (int i = 0; i < set->size; i++) {
        if (set->points[i].x == p.x && set->points[i].y == p.y) {
            return 1;
        }
    }
    return 0;
}

void merge_sets(Set *set1, Set *set2) {
    for (int i = 0; i < set2->size; i++) {
        if (!in_set(set1, set2->points[i])) {
            add_to_set(set1, set2->points[i]);
        }
    }
}

void edge(Set *ps, Set *result, int part) {
    for (int i = 0; i < ps->size; i++) {
        Point p = ps->points[i];
        Point directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (int d = 0; d < 4; d++) {
            Point n = {p.x + directions[d].x, p.y + directions[d].y};
            if (n.x >= 0 && n.x < grid_size_x && n.y >= 0 && n.y < grid_size_y && !in_set(ps, n)) {
                add_to_set(result, (Point){p.x, p.y});
            }
        }
    }
}

int main() {
    FILE *file = fopen("input12.txt", "r");
    char line[MAX_SIZE];
    
    while (fgets(line, sizeof(line), file)) {
        for (int j = 0; line[j] != '\0' && line[j] != '\n'; j++) {
            grid[grid_size_x][j] = line[j];
            grid_size_y = j + 1;
        }
        grid_size_x++;
    }
    fclose(file);

    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            Point p = {i, j};
            Set new_set;
            new_set.size = 0;
            add_to_set(&new_set, p);
            sets[set_count++] = new_set;
        }
    }

    for (int i = 0; i < set_count; i++) {
        Set *current_set = &sets[i];
        for (int j = 0; j < current_set->size; j++) {
            Point p = current_set->points[j];
            Point neighbors[] = {{p.x + 1, p.y}, {p.x - 1, p.y}, {p.x, p.y + 1}, {p.x, p.y - 1}};
            for (int n = 0; n < 4; n++) {
                if (neighbors[n].x >= 0 && neighbors[n].x < grid_size_x && neighbors[n].y >= 0 && neighbors[n].y < grid_size_y) {
                    if (grid[p.x][p.y] == grid[neighbors[n].x][neighbors[n].y]) {
                        merge_sets(current_set, &sets[i]);
                    }
                }
            }
        }
    }

    for (int part = 0; part < 2; part++) {
        int total = 0;
        for (int i = 0; i < set_count; i++) {
            Set *s = &sets[i];
            Set edge_set;
            edge(s, &edge_set, part);
            total += s->size * edge_set.size;
        }
        printf("%d\n", total);
    }

    return 0;
}

