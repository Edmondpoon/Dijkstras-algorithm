#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <inttypes.h>

#define INF -999
#define BLOCKED -1
typedef struct node node;

node *createNode(uint32_t x, uint32_t y, uint32_t seed);

void freeNode(node **n);

void addEdge(node *n, uint8_t position, int64_t weight);

int64_t getEdge(node *n, int8_t position);

int32_t getX(node *n);

int32_t getY(node *n);

bool visited(node *n);

void visit(node *n);

int64_t cost(node *n);

void updateCost(node *n, int64_t update);

#endif
