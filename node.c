#include "node.h"
#include <stdlib.h>

struct node {
    uint32_t x;
    uint32_t y;
    bool visited;
    uint64_t edges[8];
    int64_t cost; 
};

node *createNode(uint32_t y, uint32_t x, uint32_t seed) {
    node *n = (node *) calloc(1, sizeof(node));
    if (n) {
        n->x = x;
        n->y = y;
        n->cost = INF;
        n->visited = false;
        srandom(seed);
        for (int pos = 0; pos < 8; pos++) {
            n->edges[pos] = random() & 0x0000FF;
            n->edges[pos] = 1;
        }
    }
    return n;
}

void freeNode(node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

void addEdge(node *n, uint8_t position, int64_t weight) {
    n->edges[position] = weight;
    return;
}

int64_t getEdge(node *n, int8_t position) {
    if (position > -1 && n->edges[position] > 0 && !n->visited) {
        return n->edges[position];
    } else {
        return -1;
    }
}

int32_t getX(node *n) {
    return n->x;
}

int32_t getY(node *n) {
    return n->y;
}

bool visited(node *n) {
    return n->visited;
}

void visit(node *n) {
    n->visited = true;
    return;
}

int64_t cost(node *n) {
    return n->cost;
}

void updateCost(node *n, int64_t update) {
    n->cost = update;
    return;
}
