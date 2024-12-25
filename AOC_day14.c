#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 101
#define HEIGHT 103
#define MAX_ROBOTS 1000

typedef struct {
    int px, py, vx, vy;
} Robot;

Robot robots[MAX_ROBOTS];
int robot_count = 0;

void get_position_after_time(Robot robot, int t, int *new_x, int *new_y) {
    *new_x = (robot.px + robot.vx * t) % WIDTH;
    *new_y = (robot.py + robot.vy * t) % HEIGHT;
}

int get_quadrant(int px, int py) {
    if (px == WIDTH / 2 || py == HEIGHT / 2) {
        return -1;
    }
    return (px < WIDTH / 2) | ((py < HEIGHT / 2) << 1);
}

int count_horizontal_adjacencies(int positions[][2], int count) {
    int adjacencies = 0;
    int pos_set[WIDTH][HEIGHT] = {0};

    for (int i = 0; i < count; i++) {
        pos_set[positions[i][0]][positions[i][1]] = 1;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH - 1; x++) {
            if (pos_set[x][y] && pos_set[x + 1][y]) {
                adjacencies++;
            }
        }
    }
    return adjacencies;
}

int main() {
    FILE *f = fopen("input14.txt", "r");
    

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        int px, py, vx, vy;
        sscanf(line, "position=< %d, %d> velocity=< %d, %d>", &px, &py, &vx, &vy);
        robots[robot_count++] = (Robot){px, py, vx, vy};
    }
    fclose(f);

    int quadrants[4] = {0}; // 00, 01, 10, 11
    for (int i = 0; i < robot_count; i++) {
        int new_x, new_y;
        get_position_after_time(robots[i], 100, &new_x, &new_y);
        int quadrant = get_quadrant(new_x, new_y);
        if (quadrant != -1) {
            quadrants[quadrant]++;
        }
    }

    int safety_factor = 1;
    for (int i = 0; i < 4; i++) {
        safety_factor *= quadrants[i];
    }
    printf("%d\n", safety_factor);

    for (int t = 0; t < 10000; t++) {
        int positions[MAX_ROBOTS][2];
        for (int i = 0; i < robot_count; i++) {
            get_position_after_time(robots[i], t, &positions[i][0], &positions[i][1]);
        }
        int adjacencies = count_horizontal_adjacencies(positions, robot_count);
        if (adjacencies > 200) {
            printf("%d\n", t);
            break;
        }
    }

    return 0;
}

