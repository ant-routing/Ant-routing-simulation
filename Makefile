Pheromone_trees: Pheromone_trees.o tree_functions.o Definitions.o
	gcc Pheromone_trees.o tree_functions.o Definitions.o -lm -o Pheromone_trees

Definitions.o: Definitions.c
			gcc -c -Wall Definitions.c

tree_functions.o:  tree_functions.c Definitions.o
			gcc -c  -Wall tree_functions.c
