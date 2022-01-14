#include "node.h"
#include <stdlib.h>

struct node {
    uint32_t x;
    uint32_t y;
    bool visited;
    uint64_t edges[8];
    int32_t cost;
};

enum Positions {
    TOP_LEFT,
    UP,
    TOP_RIGHT,
    LEFT,
    RIGHT,
    BOT_LEFT,
    DOWN,
    BOT_RIGHT,
};

node *createNode(uint32_t x, uint32_t y, uint32_t seed) {
    node *n = (node *) calloc(1, sizeof(node));
    if (n) {
        n->x = x;
        n->y = y;
        n->cost = INF;
        n->visited = false;
        srandom(seed);
        for (int pos = 0; pos < 8; pos++) {
            n->edges[pos] = random();
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

void addEdge(node *n, uint8_t position, int64_t cost) {
    n->edges[position] = cost;
    return;
}

int64_t getEdge(node *n, uint8_t position) {
    return n->edges[position];
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

int32_t cost(node *n) {
    return n->cost;
}

void updateCost(node *n, int32_t update) {
    n->cost = update;
    return;
}
