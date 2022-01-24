#include "node.h"
#include <stdlib.h>
#include <stdio.h>

struct node {
    uint32_t x;
    uint32_t y;
    bool visited;
    uint64_t edges[8];
    int64_t cost; 
    node *parent;
};

node *create_node(uint32_t y, uint32_t x, int32_t seed) {
    node *n = (node *) calloc(1, sizeof(node));
    if (n) {
        n->x = x;
        n->y = y;
        n->cost = INF;
        n->visited = false;
        n->parent = NULL;
        if (seed >= 0) {
            srandom(seed);
        }
        for (int pos = 0; pos < 8; pos++) {
            n->edges[pos] = seed < 0 ? 1 : random() & 0x00000F;
        }
    }
    return n;
}

void free_node(node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

void add_edge(node *n, uint8_t position, int64_t weight) {
    n->edges[position] = weight;
    return;
}

int64_t get_edge(node *n, int8_t position) {
    if (position > -1 && n->edges[position] > 0 && !n->visited) {
        return n->edges[position];
    } else {
        return -1;
    }
}

void add_parent(node *n, node *parent) {
    n->parent = parent;
    return;
}

node *get_parent(node *n) {
    return n->parent;
}

int32_t get_X(node *n) {
    return n->x;
}

int32_t get_Y(node *n) {
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

void update_cost(node *n, int64_t update) {
    n->cost = update;
    return;
}
