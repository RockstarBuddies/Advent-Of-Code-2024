#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1000

typedef struct {
    char character;
    int coordinates[2];
} Antenna;

int findSlope(int antA[2], int antB[2], int *rise, int *run) {
    *rise = abs(antB[1] - antA[1]);
    *run = abs(antB[0] - antA[0]);
    return 0;
}

void setAntinode(int antA[2], int antB[2], char antinodeA[20], char antinodeB[20]) {
    int rise, run;
    findSlope(antA, antB, &rise, &run);
    
    int x1 = antA[0], y1 = antA[1];
    int x2 = antB[0], y2 = antB[1];

    if (x1 > x2) {
        x1 += run;
        x2 -= run;
    } else if (x1 < x2) {
        x1 -= run;
        x2 += run;
    }

    if (y1 < y2) {
        y1 -= rise;
        y2 += rise;
    }

    sprintf(antinodeA, "%d,%d", x1, y1);
    sprintf(antinodeB, "%d,%d", x2, y2);
}

void setAntinodePart2(int antA[2], int antB[2], int xLimit, int yLimit, char antinodes[MAX_SIZE][20], int *count) {
    int rise, run;
    findSlope(antA, antB, &rise, &run);
    
    int x1 = antA[0], y1 = antA[1];
    int x2 = antB[0], y2 = antB[1];

    sprintf(antinodes[(*count)++], "%d,%d", x1, y1);
    sprintf(antinodes[(*count)++], "%d,%d", x2, y2);
    
    bool inBounds1 = true, inBounds2 = true;
    while (inBounds1 || inBounds2) {
        if (x1 > x2) {
            x1 += run;
            x2 -= run;
        } else if (x1 < x2) {
            x1 -= run;
            x2 += run;
        }

        if (y1 < y2) {
            y1 -= rise;
            y2 += rise;
        }

        if (x1 < 0 || x1 > xLimit || y1 < 0 || y1 > yLimit) {
            inBounds1 = false;
        } else {
            sprintf(antinodes[(*count)++], "%d,%d", x1, y1);
        }

        if (x2 < 0 || x2 > xLimit || y2 < 0 || y2 > yLimit) {
            inBounds2 = false;
        } else {
            sprintf(antinodes[(*count)++], "%d,%d", x2, y2);
        }
    }
}

int main(int argc, char *argv[]) {
    int part1 = 0, part2 = 0;
    Antenna antennaMap[MAX_SIZE];
    char totalMap[MAX_SIZE][MAX_SIZE];
    char frequencies[MAX_SIZE][2];
    char antinodeList[MAX_SIZE][20];
    char antinodeList2[MAX_SIZE][20];
    int freqCount = 0, antinodeCount = 0, antinodeCount2 = 0;

    FILE *file = fopen("input8.txt", "r");
    

    int y = 0;
    while (fgets(totalMap[y], MAX_SIZE, file)) {
        for (int x = 0; totalMap[y][x] != '\0'; x++) {
            if (totalMap[y][x] != '\n' && totalMap[y][x] != '.') {
                bool found = false;
                for (int f = 0; f < freqCount; f++) {
                    if (frequencies[f][0] == totalMap[y][x]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    frequencies[freqCount][0] = totalMap[y][x];
                    frequencies[freqCount++][1] = '\0';
                }
                antennaMap[y * MAX_SIZE + x].character = totalMap[y][x];
                antennaMap[y * MAX_SIZE + x].coordinates[0] = x;
                antennaMap[y * MAX_SIZE + x].coordinates[1] = y;
            }
        }
        y++;
    }
    fclose(file);

    for (int f = 0; f < freqCount; f++) {
        int antennas[MAX_SIZE][2];
        int antennaCount = 0;
        for (int a = 0; a < MAX_SIZE; a++) {
            if (antennaMap[a].character == frequencies[f][0]) {
                antennas[antennaCount][0] = antennaMap[a].coordinates[0];
                antennas[antennaCount][1] = antennaMap[a].coordinates[1];
                antennaCount++;
            }
        }

        for (int i = 0; i < antennaCount; i++) {
            for (int b = i + 1; b < antennaCount; b++) {
                char antinodeA[20], antinodeB[20];
                setAntinode(antennas[i], antennas[b], antinodeA, antinodeB);
                strcpy(antinodeList[antinodeCount++], antinodeA);
                strcpy(antinodeList[antinodeCount++], antinodeB);
            }
        }
    }

    // Part 2
    for (int f = 0; f < freqCount; f++) {
        int antennas[MAX_SIZE][2];
        int antennaCount = 0;
        for (int a = 0; a < MAX_SIZE; a++) {
            if (antennaMap[a].character == frequencies[f][0]) {
                antennas[antennaCount][0] = antennaMap[a].coordinates[0];
                antennas[antennaCount][1] = antennaMap[a].coordinates[1];
                antennaCount++;
            }
        }

        for (int i = 0; i < antennaCount; i++) {
            for (int b = i + 1; b < antennaCount; b++) {
                char antinodes[MAX_SIZE][20];
                int count = 0;
                setAntinodePart2(antennas[i], antennas[b], MAX_SIZE, MAX_SIZE, antinodes, &count);
                for (int j = 0; j < count; j++) {
                    strcpy(antinodeList2[antinodeCount2++], antinodes[j]);
                }
            }
        }
    }

    for (int a = 0; a < antinodeCount; a++) {
        int x, y;
        sscanf(antinodeList[a], "%d,%d", &x, &y);
        if (x < 0 || x >= MAX_SIZE || y < 0 || y >= MAX_SIZE) continue;
        part1++;
        totalMap[y][x] = '#';
    }

    for (int a = 0; a < antinodeCount2; a++) {
        int x, y;
        sscanf(antinodeList2[a], "%d,%d", &x, &y);
        if (x < 0 || x >= MAX_SIZE || y < 0 || y >= MAX_SIZE) continue;
        part2++;
        totalMap[y][x] = '#';
    }

    printf("\n\nPart 1: %d   |   Part 2: %d\n", part1, part2);
    return 0;
}

