#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "Definitions.h"



struct PHEROMONE *search_SEED(int seed,struct PHEROMONE *tree);

struct PHEROMONE *rightRotate(struct PHEROMONE *y);

struct PHEROMONE *leftRotate(struct PHEROMONE *x);

int height(struct PHEROMONE *N);

int max(int a, int b);

int getBalance(struct PHEROMONE *N);

struct PHEROMONE* newNode(struct PHEROMONE_MESSAGE, int);

struct PHEROMONE* balanced_insert_SEED(struct PHEROMONE_MESSAGE, int, struct PHEROMONE* node);

void clear_tree(struct PHEROMONE *tree);
