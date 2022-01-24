#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <inttypes.h>

#define INF -999
#define BLOCKED -1
typedef struct node node;

node *create_node(uint32_t x, uint32_t y, int32_t seed);

void free_node(node **n);

void add_edge(node *n, uint8_t position, int64_t weight);

int64_t get_edge(node *n, int8_t position);

void add_parent(node *n, node *parent);

node *get_parent(node *n);

int32_t get_X(node *n);

int32_t get_Y(node *n);

bool visited(node *n);

void visit(node *n);

int64_t cost(node *n);

void update_cost(node *n, int64_t update);

#endif
