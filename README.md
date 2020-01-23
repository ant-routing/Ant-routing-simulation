# Ant-routing-simulation

This program simulates the main tasks of a node in the ant routing algorithm: look-up and insertion in AVL trees.

The main program is in the file Pheromone_trees.c. Definitions.c contains the definition of the "pheromone" structure.
The file tree_functions.c contains all functions necessary for managing AVL trees.

Use the provided makefile to compile.

The main program randomly generates a tree of pheromone seeds, of size chosen by the user. Then the program offers several possibilities for measuring
the time taken by different operations: insertion, look-up, cleaning (erasing the tree).
