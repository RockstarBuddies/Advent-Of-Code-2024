#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

char lines[MAX_LINES][MAX_LINE_LENGTH];
int line_count = 0;

void read_lines(const char *filename) {
    FILE *f = fopen(filename, "r");
    
    
    while (fgets(lines[line_count], sizeof(lines[line_count]), f) != NULL) {
        lines[line_count][strcspn(lines[line_count], "\n")] = 0; // Remove newline
        line_count++;
    }
    fclose(f);
}

void solve(int part) {
    long long tokens = 0;
    long long add = (part == 2) ? 10000000000000 : 0;
    int x1, y1, x2, y2;
    int found_a = 0, found_b = 0;

    for (int i = 0; i < line_count; i++) {
        if (strncmp(lines[i], "Button", 6) == 0) {
            char *l = lines[i];
            char a = l[7]; // 'A' or 'B'
            if (a == 'A') {
                sscanf(l + 9, " (%d,%d)", &x1, &y1);
                found_a = 1;
            } else {
                sscanf(l + 9, " (%d,%d)", &x2, &y2);
                found_b = 1;
            }
        } else if (strncmp(lines[i], "Prize", 5) == 0) {
            long long c, d;
            sscanf(lines[i], "Prize (%lld,%lld)", &c, &d);
            c += add;
            d += add;

            if (found_a && found_b) {
                double a = (c * y2 - d * x2) / (x1 * y2 - y1 * x2);
                double b = (d * x1 - c * y1) / (x1 * y2 - y1 * x2);
                if (a == (int)a && b == (int)b) {
                    tokens += (long long)(3 * a + b);
                }
            }
        }
    }

    printf("%lld\n", tokens);
}

int main() {
    read_lines("input13.txt");
    solve(1);
    solve(2);
    return 0;
}

