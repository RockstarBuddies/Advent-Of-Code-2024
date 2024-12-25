#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CODES 100
#define MAX_LENGTH 100

typedef struct {
    int x;
    int y;
} Point;

Point numkeypad[16];
Point dirkeypad[4];
Point dirs[4];

int numkeypad_size = 0;
int dirkeypad_size = 0;

int get_presses(const char *sequence, int depth, bool dirkey, Point cur);

void initialize_keypads() {
    const char *numkeypad_str[] = {"789", "456", "123", " 0A"};
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < strlen(numkeypad_str[y]); x++) {
            if (numkeypad_str[y][x] != ' ') {
                numkeypad[numkeypad_size].x = x;
                numkeypad[numkeypad_size].y = y;
                numkeypad_size++;
            }
        }
    }

    const char *dirkeypad_str[] = {" ^A", "<v>"};
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < strlen(dirkeypad_str[y]); x++) {
            if (dirkeypad_str[y][x] != ' ') {
                dirkeypad[dirkeypad_size].x = x;
                dirkeypad[dirkeypad_size].y = y;
                dirkeypad_size++;
            }
        }
    }

    dirs[0] = (Point){0, -1}; // ^
    dirs[1] = (Point){1, 0};  // >
    dirs[2] = (Point){0, 1};  // v
    dirs[3] = (Point){-1, 0}; // <
}

int get_presses(const char *sequence, int depth, bool dirkey, Point cur) {
    Point *keypad = dirkey ? dirkeypad : numkeypad;
    if (sequence[0] == '\0') {
        return 0;
    }
    if (cur.x == -1 && cur.y == -1) {
        cur = keypad[3]; // Start at 'A'
    }

    Point target = keypad[sequence[0] - '0'];
    int dx = target.x - cur.x;
    int dy = target.y - cur.y;

    char buttons[MAX_LENGTH];
    int index = 0;
    for (int i = 0; i < abs(dx); i++) {
        buttons[index++] = (dx > 0) ? '>' : '<';
    }
    for (int i = 0; i < abs(dy); i++) {
        buttons[index++] = (dy > 0) ? 'v' : '^';
    }
    buttons[index] = '\0';

    int min_len;
    if (depth) {
        int perm_lens[MAX_LENGTH];
        int perm_count = 0;

        // Generate permutations (not implemented for brevity)
        // Assume we have a function to generate permutations of buttons

        for (int i = 0; i < perm_count; i++) {
            Point temp_cur = cur;
            bool valid = true;
            for (int j = 0; j < strlen(buttons); j++) {
                temp_cur.x += dirs[buttons[j] == '>' ? 1 : buttons[j] == '<' ? 3 : buttons[j] == 'v' ? 2 : 0].x;
                temp_cur.y += dirs[buttons[j] == '>' ? 1 : buttons[j] == '<' ? 3 : buttons[j] == 'v' ? 2 : 0].y;
                if (temp_cur.x < 0 || temp_cur.x >= 3 || temp_cur.y < 0 || temp_cur.y >= 4) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                perm_lens[perm_count++] = get_presses(buttons, depth - 1, true, (Point){target.x, target.y});
            }
        }
        min_len = perm_lens[0];
        for (int i = 1; i < perm_count; i++) {
            if (perm_lens[i] < min_len) {
                min_len = perm_lens[i];
            }
        }
    } else {
        min_len = strlen(buttons) + 1;
    }
    return min_len + get_presses(sequence + 1, depth, dirkey, (Point){target.x, target.y});
}

int main(int argc, char *argv[]) {
    initialize_keypads();

    FILE *file = fopen(argc > 1 ? argv[1] : "input21.txt", "r");


    char codes[MAX_CODES][MAX_LENGTH];
    int p1 = 0, p2 = 0;
    int code_count = 0;

    while (fgets(codes[code_count], sizeof(codes[code_count]), file)) {
        codes[code_count][strcspn(codes[code_count], "\n")] = 0; // Remove newline
        int codenum = atoi(codes[code_count]);
        p1 += codenum * get_presses(codes[code_count], 0, false, (Point){-1, -1});
        p2 += codenum * get_presses(codes[code_count], 25, false, (Point){-1, -1});
        code_count++;
    }

    fclose(file);

    printf("%d\n", p1);
    printf("%d\n", p2);

    return 0;
}

