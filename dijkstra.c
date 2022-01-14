#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "hd:i:o:s:"
#define ABS(x) (x > 0 ? x : -x)
bool valid_input(char *optarg, uint32_t *variable);
void help_message(char *error);
uint8_t position(uint32_t sX, uint32_t sY, uint32_t eX, uint32_t eY );

enum Files { INFILE, OUTFILE };

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
   
    char buffer[4096]; 
    uint32_t soX, soY, destX, destY ; // Source and Destination //TODO
    uint8_t validMarkers = 0;
    uint32_t sourceX, sourceY, endX, endY;
    int64_t cost = 0;
    while (fgets(buffer, 4096, files[INFILE]) != NULL) {
        if (validMarkers == 0) {
            sscanf(buffer, "%" SCNu32" %" SCNu32 "\n", &soX, &soY);
        } else if (validMarkers == 1) {
            sscanf(buffer, "%" SCNu32" %" SCNu32 "\n", &destX, &destY);
        } else {
            sscanf(buffer, "%" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNu32 " %" SCNd64 "\n", &sourceX, &sourceY, &endX, &endY, &cost);
            addEdge(board[sourceY][sourceX], position(sourceX, sourceY, endX, endY), cost > 0 ? cost : BLOCKED);
        }
    }
    
    updateCost(board[soX][soY], 0); 
    
    // TODO print path and entire board once either no path or found path (make path module)
    while () 


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

uint8_t position(uint32_t sX, uint32_t sY, uint32_t eX, uint32_t eY ) {
    int8_t deltaX = eX - sX, deltaY = eY - sY;
    int8_t sum = deltaX + deltaY; 
    uint8_t position;
    switch (ABS(sum)) {
    case 0:
        position = deltaX > 0 ? 2 : 5; // top right or bottom left
        break;
    case 2:
        position = deltaX > 0 ? 7 : 0; // top left or bottom right
        break;
    case 4:
        if (deltaX == 0) { 
            position = deltaY > 0 ? 6 : 1; // down or up
        } else {
            position = deltaX > 0 ? 4 : 3; // right ot left
        }
        break;
    default:;
    }
    return position;
}
