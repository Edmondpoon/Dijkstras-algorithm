# Dijkstra's Algorithm
Implementation of Dijkstra's algorithm in C. This implementation allows for the user to specify edges and "holes" in an NxN matrix.

# Compilation
- `make` or `make all` will compile all C source files into an executable `dijkstra`.
- `make clean` will clean/remove all created object files and the final executable.

# Available flags
|Flag                   |Description                                                        | 
|:---------------------:| ----------------------------------------------------------------- |
|c                      |Prints the final costs of each node after the algorithm is run.    |
|h                      |Prints the usage for the program.                                  |
|f                      |Sets the weight of all edges to 1.                                 |
|d <em> dimension </em> |Sets the dimension of the NxN matrix (default: 12).                |
|i <em> infile    </em> |Sets the file to read edges/nodes from (default: stdin).           |
|o <em> outfile   </em> |Sets the file to write the data/paths to (default: stdout).        |
|s <em> seed      </em> |Sets the seed used to randomize the edge weights (default: 1337).  |

