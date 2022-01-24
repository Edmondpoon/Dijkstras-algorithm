#include "node.h"
#include <stdlib.h>

struct node {
    uint32_t x;
    uint32_t y;
    bool visited;
    uint64_t edges[8];
    int64_t cost; 
    node *parent;
};

// 
// Dynamically allocates memory for a node and its associated members
//
// y: Associated y value for the node
// x: Associated x value for the node
// seed: Seed used to generate the random edge weights
//
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

//
// Frees the memory associated with a node
//
// n: The node to free
//
void free_node(node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

//
// Updates the weight from a node to another node
//
// n: the starting node of the edge
// position: the position of the ending node 
//      -------------
//      | 0 | 1 | 2 |
//      | 3 | X | 4 |
//      | 5 | 6 | 7 |
//      -------------
// weight: the weight of the edge
//
void add_edge(node *n, uint8_t position, int64_t weight) {
    n->edges[position] = weight;
    return;
}

//
// Gets the weight from a node to another node
//
// n: the starting node of the edge
// position: the position of the ending node 
//      -------------
//      | 0 | 1 | 2 |
//      | 3 | X | 4 |
//      | 5 | 6 | 7 |
//      -------------
//
int64_t get_edge(node *n, int8_t position) {
    if (position > -1 && n->edges[position] > 0 && !n->visited) {
        return n->edges[position];
    } else {
        return -1;
    }
}

// 
// Adds a node as the parent of another node
//
// n: the node to update the parent of
// parent: the parent node
//
void add_parent(node *n, node *parent) {
    n->parent = parent;
    return;
}

//
// Gets the parent of a node
//
// n: the node to get the parent of
//
node *get_parent(node *n) {
    return n->parent;
}

// 
// Gets the x value of a node
//
// n: the node to check
// 
int32_t get_X(node *n) {
    return n->x;
}

// 
// Gets the y value of a node
//
// n: the node to check
// 
int32_t get_Y(node *n) {
    return n->y;
}

// 
// Returns whether the node has been visited or not
//
// n: the node to check
//
bool visited(node *n) {
    return n->visited;
}

//
// Marks a node as visited
//
// n: the node to update
void visit(node *n) {
    n->visited = true;
    return;
}

//
// Returns the cost of a node
//
// n: the node desired
//
int64_t cost(node *n) {
    return n->cost;
}

//
// Updates the cost of a node
//
// n: the node to update
// update: the new node cost
//
void update_cost(node *n, int64_t update) {
    n->cost = update;
    return;
}
