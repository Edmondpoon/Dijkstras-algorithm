#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "hd:i:o:s:"
#define ABS(x) (x > 0 ? x : -x)

bool valid_input(char *optarg, uint32_t *variable);
void help_message(char *error);
int8_t findPosition(int8_t deltaX, int8_t deltaY);
bool inRange(uint32_t dim, uint32_t x, uint32_t y);

enum Files { INFILE, OUTFILE };
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
    int8_t opt = 0;
    uint32_t dimension = 12, seed = 1337;
    FILE *files[2] = { stdin, stdout };
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'd': // dimensions
            valid_input(optarg, &dimension);
            break;
        case 's': // random seed
            valid_input(optarg, &seed);
            break;
        case 'i': // infile
            files[INFILE] = fopen(optarg, "r");
            if (!files[INFILE]) {
                ;//TODO error
            }
            break;
        case 'o': // outfile
            files[OUTFILE] = fopen(optarg, "w");
            if (!files[OUTFILE]) {
                ;//TODO error
            }
            break;
        case 'h': // help
            break;
        default:
            return 1;
        }
    }

    // create the board
    node *board[dimension][dimension];
    for (uint32_t row = 0; row < dimension; row++) {
        for (uint32_t col = 0; col < dimension; col++) {
           board[row][col] = createNode(row, col, seed); 
        }
    }
   
    int8_t position;
    char buffer[4096]; 
    uint32_t soX, soY, destX, destY ; // Source and Destination //TODO
    uint8_t validMarkers = 0;
    uint32_t sourceX, sourceY, endX, endY;
    int64_t weight = 0;
    while (fgets(buffer, 4096, files[INFILE]) != NULL) {
        // TODO to ensure start/end nodes are valid
        if (validMarkers == 0) {
            sscanf(buffer, "%" SCNu32" %" SCNu32 "\n", &soX, &soY);
            validMarkers += 1;
        } else if (validMarkers == 1) {
            sscanf(buffer, "%" SCNu32" %" SCNu32 "\n", &destX, &destY);
            validMarkers += 1;
        } else {
            sscanf(buffer, "%" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNd64 "\n", &sourceX, &sourceY, &endX, &endY, &weight);
            position = findPosition(sourceX - endX, sourceY - endY);
            if (position > 0) {
                addEdge(board[sourceY][sourceX], position, weight > 0 ? weight : BLOCKED);
            }
        }
    }
    
    node *next, *current = board[soY][soX], *destination = board[destY][destX], *min;
    updateCost(current, 0); 
    // TODO print path and entire board once either no path or found path (make path module)
    while (!visited(destination)) {
        printf("---------------------\n");
        printf("X, y: %" PRIu32 "%" PRIu32 "\n", getX(current), getY(current));
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                if (inRange(dimension, getX(current) + x, getY(current) + y)) {
                    printf("x = %d y = %d inRange\n", x, y);
                    next = board[getY(current) + y][getX(current) + x];
                    weight = getEdge(current, findPosition(x, y));
                    printf("weight = %"PRId64"\n", weight);
                    if (!visited(next) && weight >= 0 && (cost(next) > weight + cost(current) || cost(next) == INF)) {
                        printf("check allowed\n");
                        printf("looking at node X = %"PRIu32 " Y = %"PRIu32"\n", getX(next), getY(next));
                        printf("minimum node X = %"PRIu32 " Y = %"PRIu32"\n", getX(min), getY(min));
                        printf("old cost = %" PRId64 "\n", cost(next));
                        updateCost(next, weight + cost(current));
                        printf("old cost = %" PRId64 "\n", cost(next));
                    }
                }
            }
        }
        visit(current);
        printf("visited %" PRIu32 " %" PRIu32 "\n", getX(current), getY(current));
        // no possible next nodes

        min = NULL;
        current = NULL;
        for (uint32_t row = 0; row < dimension; row++) {
            for (uint32_t col = 0; col < dimension; col++) {
                min = board[row][col];
                if (cost(min) < 0 || visited(min)) {
                    continue;
                } else if (!current || cost(current) > cost(min)) {
                    current = min;
                }
            }
        }
        if (!current) {
            break;
        }
        printf("new current: %" PRIu32 " %" PRIu32"\n", getX(current), getY(current));
    } 
    /*
    while (!visited(destination)) {
        min = NULL;
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                int posX = getX(current) + x, posY = getY(current) + y;
                if (inRange(dimension, posX, posY)) {
                    next = board[posY][posX];
                    weight = getEdge(current, findPosition(x, y));
                    if (!visited(next) && weight > 0) {
                        int newCost = cost(current) + weight;
                        if (cost(next) < 0 || newCost < cost(next)) {
                            updateCost(next, newCost);
                        } 
                        if (!min || cost(min) > newCost) {
                            min = next;
                        }
                    }
                }
            }
        }
        visit(current);
        printf("visited %" PRIu32 " %" PRIu32 "\n", getX(current), getY(current));
        // no possible next nodes
        if (!min) {
            printf("broke out of loop\n");
            break;
        } else {
            current = min;
            printf("new current: %" PRIu32 " %" PRIu32"\n", getX(current), getY(current));
        }
    }
    */
    int64_t temp = 0; //TODO
    for (uint32_t row = 0; row < dimension; row++) {
        for (uint32_t col = 0; col < dimension; col++) {
            temp = cost(board[row][col]);
            if (temp < 0) {
                printf(" *** ");
            } else {
                printf("%4" PRId64 " ", cost(board[row][col]));
            }

        }
        printf("\n");
    }
    
    printf("%" PRId64"\n", cost(destination));

    return EXIT_SUCCESS;
}

void help_message(char *error) {
    if (*error != '\0') {
        fprintf(stderr, "%s", error);
    }
    fprintf(stderr, "SYNOPSIS\n"
                    "  A word filtering program for the GPRSC.\n"
                    "  Filters out and reports bad words parsed from stdin.\n\n"
                    "USAGE\n"
                    "  ./banhammer [-hs] [-t size] [-f size]\n\n"
                    "OPTIONS\n"
                    "  -h           Program usage and help.\n"
                    "  -s           Print program statistics.\n"
                    "  -t size      Specify hash table size (default: 2^16).\n"
                    " other not specified edges will have random weight"
                    "  -f size      Specify Bloom filter size (default: 2^20).\n");
    return;
}


bool valid_input(char *optarg, uint32_t *variable) {
    // if the argument for this flag contains a character or is less than 0, print the help message
    char *invalid;
    int64_t temp_input = strtoul(optarg, &invalid, 10);
    if ((invalid != NULL && *invalid != '\0') || temp_input < 0) {
        help_message("Invalid argument for specified flag.\n");
        return false;
    }
    *variable = (uint32_t) temp_input;
    return true;
}

int8_t findPosition(int8_t deltaX, int8_t deltaY) {
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

bool inRange(uint32_t dim, uint32_t x, uint32_t y) {
    return (x >= 0 && x < dim && y < dim && y >= 0);
}
