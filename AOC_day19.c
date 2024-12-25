#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "input19.txt"
#define MAX_PATTERNS 100
#define MAX_DESIGNS 100
#define MAX_LENGTH 100

typedef struct {
    char patterns[MAX_PATTERNS][MAX_LENGTH];
    int count;
} PatternSet;

typedef struct {
    char designs[MAX_DESIGNS][MAX_LENGTH];
    int count;
} DesignSet;

PatternSet parse_input(const char *filename) {
    PatternSet patternSet;
    DesignSet designSet;
    patternSet.count = 0;
    designSet.count = 0;

    FILE *input_file = fopen(filename, "r");
    

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        if (strcmp(line, "\n") == 0) break;
        char *token = strtok(line, ", ");
        while (token != NULL) {
            strcpy(patternSet.patterns[patternSet.count++], token);
            token = strtok(NULL, ", ");
        }
    }

    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0;
        strcpy(designSet.designs[designSet.count++], line);
    }

    fclose(input_file);
    return patternSet;
}

int remove_prefix(const char *prefix, const char *word, char *result) {
    if (strncmp(prefix, word, strlen(prefix)) == 0) {
        strcpy(result, word + strlen(prefix));
        return 1;
    }
    return 0;
}

int check_design(const char *design, PatternSet patterns) {
    if (strlen(design) == 0) {
        return 1;
    }
    int count = 0;
    for (int i = 0; i < patterns.count; i++) {
        char new_design[MAX_LENGTH];
        if (remove_prefix(patterns.patterns[i], design, new_design)) {
            count += check_design(new_design, patterns);
        }
    }
    return count;
}

int part_one(DesignSet designs, PatternSet patterns) {
    int total = 0;
    for (int i = 0; i < designs.count; i++) {
        total += check_design(designs.designs[i], patterns) > 0;
    }
    return total;
}

int part_two(DesignSet designs, PatternSet patterns) {
    int total = 0;
    for (int i = 0; i < designs.count; i++) {
        total += check_design(designs.designs[i], patterns);
    }
    return total;
}

int main() {
    PatternSet patterns = parse_input(FILENAME);
    DesignSet designs;
    designs.count = 0;

 
    FILE *input_file = fopen(FILENAME, "r");
    

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        if (strcmp(line, "\n") == 0) break;
    }

    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0; 
        strcpy(designs.designs[designs.count++], line);
    }

    fclose(input_file);

    printf("%d\n", part_one(designs, patterns));
    printf("%d\n", part_two(designs, patterns));

    return 0;
}

