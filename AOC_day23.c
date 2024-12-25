#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_NEIGHBOURS 10
#define MAX_TRIPLETS 100

typedef struct Node {
    char idx[10];
    char neighbours[MAX_NEIGHBOURS][10];
    int neighbour_count;
} Node;

typedef struct NetworkGraph {
    Node nodes[MAX_NODES];
    int node_count;
    char triplets[MAX_TRIPLETS][3][10];
    int triplet_count;
} NetworkGraph;

void init_node(Node *node, const char *idx, const char *neighbour) {
    strcpy(node->idx, idx);
    strcpy(node->neighbours[0], neighbour);
    node->neighbour_count = 1;
}

void add_neighbour(Node *node, const char *neighbour) {
    for (int i = 0; i < node->neighbour_count; i++) {
        if (strcmp(node->neighbours[i], neighbour) == 0) {
            return; // Already a neighbour
        }
    }
    strcpy(node->neighbours[node->neighbour_count++], neighbour);
}

bool is_triplet(const char *idx1, const char *idx2, const char *neighbour, NetworkGraph *graph) {
    for (int i = 0; i < graph->triplet_count; i++) {
        if ((strcmp(graph->triplets[i][0], idx1) == 0 && strcmp(graph->triplets[i][1], idx2) == 0) ||
            (strcmp(graph->triplets[i][0], idx2) == 0 && strcmp(graph->triplets[i][1], idx1) == 0)) {
            return true;
        }
    }
    return false;
}

void check_triplets(const char *idx1, const char *idx2, NetworkGraph *graph) {
    for (int i = 0; i < graph->nodes[graph->node_count - 1].neighbour_count; i++) {
        const char *neighbour = graph->nodes[graph->node_count - 1].neighbours[i];
        for (int j = 0; j < graph->nodes[graph->node_count - 2].neighbour_count; j++) {
            if (strcmp(neighbour, graph->nodes[graph->node_count - 2].neighbours[j]) == 0) {
                strcpy(graph->triplets[graph->triplet_count][0], idx1);
                strcpy(graph->triplets[graph->triplet_count][1], idx2);
                strcpy(graph->triplets[graph->triplet_count][2], neighbour);
                graph->triplet_count++;
                return;
            }
        }
    }
}

void add_nodes(NetworkGraph *graph, const char *idx1, const char *idx2) {
    bool found1 = false, found2 = false;
    for (int i = 0; i < graph->node_count; i++) {
        if (strcmp(graph->nodes[i].idx, idx1) == 0) {
            add_neighbour(&graph->nodes[i], idx2);
            found1 = true;
        }
        if (strcmp(graph->nodes[i].idx, idx2) == 0) {
            add_neighbour(&graph->nodes[i], idx1);
            found2 = true;
        }
    }
    if (!found1) {
        init_node(&graph->nodes[graph->node_count], idx1, idx2);
        graph->node_count++;
    }
    if (!found2) {
        init_node(&graph->nodes[graph->node_count], idx2, idx1);
        graph->node_count++;
    }
    check_triplets(idx1, idx2, graph);
}

int count_triplets_starting_with(NetworkGraph *graph, char starts_with) {
    int count = 0;
    for (int i = 0; i < graph->triplet_count; i++) {
        for (int j = 0; j < 3; j++) {
            if (starts_with == graph->triplets[i][j][0]) {
                count++;
                break;
            }
        }
    }
    return count;
}

bool all_connected(NetworkGraph *graph, char nodes[][10], int node_count) {
    for (int i = 0; i < node_count; i++) {
        for (int j = i + 1; j < node_count; j++) {
            bool connected = false;
            for (int k = 0; k < graph->nodes[i].neighbour_count; k++) {
                if (strcmp(graph->nodes[i].neighbours[k], nodes[j]) == 0) {
                    connected = true;
                    break;
                }
            }
            if (!connected) {
                return false;
            }
        }
    }
    return true;
}

void get_password(NetworkGraph *graph, char *result) {
    strcpy(result, "password"); // Placeholder
}

int main(int argc, char *argv[]) {
    NetworkGraph network_graph = {0};
    char file_name[100] = "input23.txt";
    if (argc > 1) {
        strcpy(file_name, argv[1]);
    }
    
    FILE *file = fopen(file_name, "r");

    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        char *idx1 = strtok(line, "-");
        char *idx2 = strtok(NULL, "-");
        add_nodes(&network_graph, idx1, idx2);
    }
    fclose(file);
    
    int triplet_count = count_triplets_starting_with(&network_graph, 't');
    printf("%d\n", triplet_count);
    
    char password[100];
    get_password(&network_graph, password);
    printf("%s\n", password);
    
    return 0;
}

