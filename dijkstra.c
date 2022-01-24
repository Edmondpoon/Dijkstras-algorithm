#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "chfd:i:o:s:"
#define ABS(x) (x > 0 ? x : -x)

bool valid_input(char *optarg, uint32_t *variable);
void help_message(void);
void free_files(FILE *files[2]);
node *scan_nodes(node ***board, char *buffer, bool edge);
int8_t find_position(int8_t deltaX, int8_t deltaY);
bool in_range(uint32_t dim, uint32_t x, uint32_t y);
void print_path(FILE *outfile, node *current, node *source);

enum FilePointers { INFILE, OUTFILE };
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

int main(int argc, char **argv) {
    bool costs = false;
    int8_t opt = 0;
    uint32_t dimension = 12, seed = 1337;
    FILE *files[2] = { stdin, stdout };
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'd': // dimensions
            valid_input(optarg, &dimension);
            break;
        case 'f': // fixed edge weights
            seed = -1;
            break;
        case 'c': // print final node costs
            costs = true;
            break;
        case 's': // random seed
            valid_input(optarg, &seed);
            break;
        case 'i': // infile
            files[INFILE] = fopen(optarg, "r");
            if (!files[INFILE]) {
                free_files(files);
                fprintf(stderr, "Unable to open the file to read from.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o': // outfile
            files[OUTFILE] = fopen(optarg, "w");
            if (!files[OUTFILE]) {
                free_files(files);
                fprintf(stderr, "Unable to open the file to write to.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'h': // help
            help_message();
            return EXIT_SUCCESS;
        default:
            return EXIT_FAILURE;
        }
    }
    // create the board
    node ***board = (node ***) calloc(dimension, sizeof(node **));
    for (uint32_t index = 0; index < dimension; index++) {
        board[index] = (node **) calloc(dimension, sizeof(node *));
    }
    for (uint32_t row = 0; row < dimension; row++) {
        for (uint32_t col = 0; col < dimension; col++) {
            board[row][col] = create_node(row, col, seed); 
        }
    }
    node *source = NULL, *destination = NULL; 
    char buffer[4096]; 
    while (fgets(buffer, 4096, files[INFILE]) != NULL) {
        if (!source) {
            source = scan_nodes(board, buffer, false);
        } else if (!destination) {
            destination = scan_nodes(board, buffer, false);
        } else {
            scan_nodes(board, buffer, true);
        }
    }
    node *next, *current = source;
    update_cost(current, 0); 
    int64_t weight = 0;
    while (!visited(destination) && current) {
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                if (in_range(dimension, get_X(current) + x, get_Y(current) + y)) {
                    next = board[get_Y(current) + y][get_X(current) + x];
                    weight = get_edge(current, find_position(x, y));
                    if (!visited(next) && weight >= 0 && (cost(next) > weight + cost(current) || cost(next) == INF)) {
                        update_cost(next, weight + cost(current));
                        add_parent(next, current);
                    }
                }
            }
        }
        visit(current);
        current = NULL;
        for (uint32_t row = 0; row < dimension; row++) {
            for (uint32_t col = 0; col < dimension; col++) {
                next = board[row][col];
                if (cost(next) < 0 || visited(next)) {
                    continue;
                } else if (!current || cost(current) > cost(next)) {
                    current = next;
                }
            }
        }
    } 
    if (costs) {
        int64_t temp = 0;
        for (uint32_t row = 0; row < dimension; row++) {
            for (uint32_t col = 0; col < dimension; col++) {
                temp = cost(board[row][col]);
                if (temp < 0) {
                    // Blocked position
                    fprintf(files[OUTFILE], " *** ");
                } else {
                    fprintf(files[OUTFILE], "%4" PRId64 " ", cost(board[row][col]));
                }
            }
            printf("\n");
        }
    }
    print_path(files[OUTFILE], destination, source);
    printf("%" PRId64"\n", cost(destination));
    return EXIT_SUCCESS;
}

// 
// Prints the usage message
//
void help_message(void) {
    printf("SYNOPSIS\n"
            " An implementation of Dijkstra's Algorithm on any given graph.\n"
            "USAGE\n"
            "  ./dijkstra [-hf] [-d dimension] [-i infile] [-o outfile] [-s seed]\n\n"
            "OPTIONS\n"
            "  -h           Program usage and help.\n"
            "  -c           Prints the final costs of each node after the algorithm finishes.\n" 
            "  -f           Sets the edges between all nodes to 1 (default: random between 1 - 15).\n" 
            "  -d dimension Sets the dimension of the NxN matrix (default: 12).\n"
            "  -i infile    The file to read the edges and points from (default: stdin).\n"
            "  -o outfile   The outfile to write the shortest path to (default: stdout).\n"
            "  -s seed      Sets the seed used to generate the random edge weights (default: 1337).\n");
    return;
}

//
// Checks if the argument for a flag was valid
//
// optarg: the argument for the flag
// variable: the variable to store the valid argument in
//
bool valid_input(char *optarg, uint32_t *variable) {
    // if the argument for this flag contains a character or is less than 0, print the help message
    char *invalid;
    int64_t temp_input = strtoul(optarg, &invalid, 10);
    if ((invalid != NULL && *invalid != '\0') || temp_input < 0) {
        fprintf(stderr, "Invalid argument for specified flag.\n");
        return false;
    }
    *variable = (uint32_t) temp_input;
    return true;
}

// 
// Finds the corresponding position for an end node
//
// deltaX: the difference between the two nodes' x-values
// deltaY: the difference between the two nodes' y-values
//
int8_t find_position(int8_t deltaX, int8_t deltaY) {
    int8_t sum = deltaX + deltaY; 
    int8_t position = -1;
    switch (ABS(sum)) {
    case 0:
        if (deltaX != 0) {
            position = deltaX > 0 ? TOP_RIGHT : BOT_LEFT; // top right or bottom left
        }
        break;
    case 2:
        position = deltaX > 0 ? BOT_RIGHT : TOP_LEFT; // top left or bottom right
        break;
    case 1:
        if (deltaX == 0) { 
            position = deltaY > 0 ? DOWN : UP; // down or up
        } else {
            position = deltaX > 0 ? RIGHT : LEFT; // right ot left
        }
        break;
    default:;
    }
    return position;
}

//
// Determines whether an x/y position is in range
//
// dim: the dimensions of the graph
// x: the x position
// y: the y position
//
bool in_range(uint32_t dim, uint32_t x, uint32_t y) {
    return (x >= 0 && x < dim && y < dim && y >= 0);
}

//
// Frees memory associated with the FILE pointers
//
// files: an array of FILE pointers
//
void free_files(FILE *files[2]) {
    if (files[INFILE]) {
        fclose(files[INFILE]);
        files[INFILE] = NULL;
    }
    if (files[OUTFILE]) {
        fclose(files[OUTFILE]);
        files[OUTFILE] = NULL;
    }
    return;
}

//
// Reads a node/edge from an instream
//
// board: a NxN atrix representing the graph
// buffer: a buffer containing a string from an instream
// edge: wheter the item read is an edge or node
node *scan_nodes(node ***board, char *buffer, bool edge) {
    static uint64_t lineNum = 0;
    uint32_t startX, startY, endX, endY;
    int64_t weight = 0;
    int8_t position, scanned;
    if (edge) {
        scanned = sscanf(buffer, "%" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNd64 "\n", &startX, &startY, &endX, &endY, &weight);
        position = find_position(startX - endX, startY - endY);
        if (position > 0) {
            add_edge(board[startY][startX], position, weight > 0 ? weight : BLOCKED);
        }
    } else {
        scanned = sscanf(buffer, "%" SCNu32" %" SCNu32 "\n", &startX, &startY);
    }
    lineNum += 1;
    node *updatedNode = board[startY][startX];
    if (scanned != 2 && scanned != 4) {
        fprintf(stderr, "Invalid node when reading from line %" PRIu64 ".\n", lineNum);
        exit(EXIT_FAILURE);
    }
    return updatedNode;
}

//
// Prints the shortest path if one was found
//
// outfile: the file to print the path to
// current: the current node we're printing
// source: the source node
//
void print_path(FILE *outfile, node *current, node *source) {
    if (cost(current) < 0) {
        printf("There is no path from source node (%" PRIu32 ", %" PRIu32 ") and destination node (%" PRIu32 ", %" PRIu32 ".\n", 
                get_X(source), get_Y(source), get_X(current), get_Y(current));
    } else if (get_parent(current) == NULL) {
        printf("X: %" PRIu32 " Y: %" PRIu32 "\n", get_X(current), get_Y(current));
    } else {
        print_path(outfile, get_parent(current), source);
        printf("X: %" PRIu32 " Y: %" PRIu32 "\n", get_X(current), get_Y(current));
    }
    return;
}
