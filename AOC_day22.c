#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_NUMS 2005
#define DIFFS_SIZE 2001
#define PATTERN_SIZE 4

uint32_t f(uint32_t s) {
    s ^= (s << 6) & 0xFFFFFF;
    s ^= (s >> 5) & 0xFFFFFF;
    return s ^ ((s << 11) & 0xFFFFFF);
}

int main() {
    FILE *file = fopen("input22.txt", "r");


    uint32_t ans1 = 0;
    uint32_t ans2[10000] = {0}; // Assuming a maximum of 10000 unique patterns
    uint32_t nums[MAX_NUMS];
    uint32_t diffs[DIFFS_SIZE];
    uint32_t seen[10000] = {0}; // To track seen patterns
    int seen_count = 0;

    while (!feof(file)) {
        uint32_t s;
        if (fscanf(file, "%u", &s) != 1) break;

        nums[0] = s;
        for (int i = 1; i <= 2000; i++) {
            s = f(s);
            nums[i] = s;
        }
        ans1 += nums[2000];

        for (int i = 0; i < 2000; i++) {
            diffs[i] = (nums[i + 1] % 10) - (nums[i] % 10);
        }

        for (int i = 0; i < 1997; i++) {
            uint32_t pat = 0;
            for (int j = 0; j < PATTERN_SIZE; j++) {
                pat = (pat << 4) | (diffs[i + j] & 0xF);
            }

            int found = 0;
            for (int j = 0; j < seen_count; j++) {
                if (seen[j] == pat) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                ans2[pat] += nums[i + 4] % 10;
                seen[seen_count++] = pat;
            }
        }
    }

    fclose(file);

    uint32_t max_ans2 = 0;
    for (int i = 0; i < 10000; i++) {
        if (ans2[i] > max_ans2) {
            max_ans2 = ans2[i];
        }
    }

    printf("%u %u\n", ans1, max_ans2);
    return 0;
}

