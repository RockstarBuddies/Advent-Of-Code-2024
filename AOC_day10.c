#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_MAP_SIZE 100
#define MAX_DIRECTIONS 4

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int** map;
    int map_rows;
    int map_cols;
    Point directions[MAX_DIRECTIONS];
    Point** paths_from_trail_heads;
    int num_trail_heads;
    bool use_recursion;
} TrailFinder;

Point create_point(int x, int y) {
    Point p = {x, y};
    return p;
}

Point point_add(Point a, Point b) {
    return create_point(a.x + b.x, a.y + b.y);
}

Point point_opposite(Point p) {
    return create_point(-p.x, -p.y);
}

bool point_equals(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

void init_trail_finder(TrailFinder* tf, char** lines, int num_lines, bool recursive) {
    tf->map_rows = num_lines;
    tf->map_cols = strlen(lines[0]);
    tf->map = (int**)malloc(tf->map_rows * sizeof(int*));
    for (int i = 0; i < tf->map_rows; i++) {
        tf->map[i] = (int*)malloc(tf->map_cols * sizeof(int));
        for (int j = 0; j < tf->map_cols; j++) {
            tf->map[i][j] = lines[i][j] - '0';
        }
    }

    tf->directions[0] = create_point(0, 1);
    tf->directions[1] = create_point(1, 0);
    tf->directions[2] = create_point(0, -1);
    tf->directions[3] = create_point(-1, 0);

    tf->use_recursion = recursive;
    tf->num_trail_heads = 0;
    tf->paths_from_trail_heads = NULL;
}

int map_height(TrailFinder* tf, Point p) {
    if (p.x >= 0 && p.x < tf->map_rows && p.y >= 0 && p.y < tf->map_cols) {
        return tf->map[p.x][p.y];
    }
    return -1;
}

Point* directions_without_opposite(TrailFinder* tf, Point direction, int* count) {
    Point* result = (Point*)malloc(MAX_DIRECTIONS * sizeof(Point));
    *count = 0;
    for (int i = 0; i < MAX_DIRECTIONS; i++) {
        if (!point_equals(tf->directions[i], point_opposite(direction))) {
            result[(*count)++] = tf->directions[i];
        }
    }
    return result;
}

Point* trail_score_recursive(TrailFinder* tf, Point point, int height, Point* directions, int num_directions, int* result_size) {
    if (height == 9) {
        Point* result = (Point*)malloc(sizeof(Point));
        result[0] = point;
        *result_size = 1;
        return result;
    }

    Point* result = NULL;
    int total_size = 0;

    for (int i = 0; i < num_directions; i++) {
        Point next_point = point_add(point, directions[i]);
        if (map_height(tf, next_point) == height + 1) {
            int new_directions_count;
            Point* new_directions = directions_without_opposite(tf, directions[i], &new_directions_count);
            int sub_result_size;
            Point* sub_result = trail_score_recursive(tf, next_point, height + 1, new_directions, new_directions_count, &sub_result_size);
            free(new_directions);

            result = (Point*)realloc(result, (total_size + sub_result_size) * sizeof(Point));
            memcpy(result + total_size, sub_result, sub_result_size * sizeof(Point));
            total_size += sub_result_size;
            free(sub_result);
        }
    }

    *result_size = total_size;
    return result;
}

Point* trail_score_loop(TrailFinder* tf, Point start_point, int start_height, int* result_size) {
    // Implementation of trail_score_loop
    // This is a placeholder and needs to be implemented
    *result_size = 0;
    return NULL;
}

void find_paths_from_trail_heads(TrailFinder* tf) {
    int trail_head_count = 0;
    Point* trail_heads = (Point*)malloc(tf->map_rows * tf->map_cols * sizeof(Point));

    for (int i = 0; i < tf->map_rows; i++) {
        for (int j = 0; j < tf->map_cols; j++) {
            if (tf->map[i][j] == 0) {
                trail_heads[trail_head_count++] = create_point(i, j);
            }
        }
    }

    tf->num_trail_heads = trail_head_count;
    tf->paths_from_trail_heads = (Point**)malloc(trail_head_count * sizeof(Point*));

    for (int i = 0; i < trail_head_count; i++) {
        int result_size;
        if (tf->use_recursion) {
            tf->paths_from_trail_heads[i] = trail_score_recursive(tf, trail_heads[i], 0, tf->directions, MAX_DIRECTIONS, &result_size);
        } else {
            tf->paths_from_trail_heads[i] = trail_score_loop(tf, trail_heads[i], 0, &result_size);
        }
    }

    free(trail_heads);
}

int scores(TrailFinder* tf) {
    int total_score = 0;
    for (int i = 0; i < tf->num_trail_heads; i++) {
        // Count unique points in each path
        // This is a placeholder and needs to be implemented
    }
    return total_score;
}

int ratings(TrailFinder* tf) {
    int total_rating = 0;
    for (int i = 0; i < tf->num_trail_heads; i++) {
        // Count total points in each path
        // This is a placeholder and needs to be implemented
    }
    return total_rating;
}

char** get_lines(const char* filename, int* num_lines) {
    FILE* file = fopen(filename, "r");
   

    char** lines = NULL;
    char buffer[256];
    *num_lines = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
        lines = (char**)realloc(lines, (*num_lines + 1) * sizeof(char*));
        lines[*num_lines] = strdup(buffer);
        (*num_lines)++;
    }

    fclose(file);
    return lines;
}

int main() {
    int num_lines;
    char** lines = get_lines("input10.txt", &num_lines);

    TrailFinder tf;
    init_trail_finder(&tf, lines, num_lines, true);
    find_paths_from_trail_heads(&tf);

    printf("1: %d\n", scores(&tf));
    printf("2: %d\n", ratings(&tf));

    // Free allocated memory
    for (int i = 0; i < tf.map_rows; i++) {
        free(tf.map[i]);
    }
    free(tf.map);
    for (int i = 0; i < tf.num_trail_heads; i++) {
        free(tf.paths_from_trail_heads[i]);
    }
    free(tf.paths_from_trail_heads);
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}

