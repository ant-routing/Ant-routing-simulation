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

extern struct PHEROMONE *PheromoneTree;

int height(struct PHEROMONE *N){
    if (N == NULL){
      return 0;
    }
    return N->height;
}

int max(int a, int b){
    return (a > b)? a : b;
}

int getBalance(struct PHEROMONE *N){
    if (N == NULL)
        return 0;
    return height(N->left_child) - height(N->right_child);
}

struct PHEROMONE *search_SEED(int seed,struct PHEROMONE *tree){ /*AVL-tree seach. returns NULL if seed is not in the tree, returns a pointer to the node labeled with seed otherwise*/
  if(tree==NULL){
    return tree;
  }
  struct PHEROMONE *node=tree;
  while(node!=NULL){
    if(seed<node->seed){
      node=node->left_child;
    }
    else if(seed>node->seed){
      node=node->right_child;
    }
    else{
      return node;
    }
  }
  return node;
}


struct PHEROMONE *rightRotate(struct PHEROMONE *y){
    struct PHEROMONE *x = y->left_child;
    struct PHEROMONE *T2 = x->right_child;

    // Perform rotation
    x->right_child = y;
    y->left_child = T2;

    // Update heights
    y->height = max(height(y->left_child), height(y->right_child))+1;
    x->height = max(height(x->left_child), height(x->right_child))+1;

    // Return new root
    return x;
}

struct PHEROMONE *leftRotate(struct PHEROMONE *x){
    struct PHEROMONE *y = x->right_child;
    struct PHEROMONE *T2 = y->left_child;

    // Perform rotation
    y->left_child = x;
    x->right_child = T2;

    //  Update heights
    x->height = max(height(x->left_child), height(x->right_child))+1;
    y->height = max(height(y->left_child), height(y->right_child))+1;

    // Return new root
    return y;
}





struct PHEROMONE* newNode(struct PHEROMONE_MESSAGE message,int sender){ //creates a new node with the seed and infos from "message", received from node identified by "sender"
    struct PHEROMONE* node = (struct PHEROMONE*)
                        malloc(sizeof(struct PHEROMONE));
    node->seed   = message.seed;
    node->left_child   = NULL;
    node->right_child  = NULL;
    node->height = 1;  // new node is initially added at leaf
    if(message.origin){
      node->bob=true;
      node->alice=false;
      node->fees[1]=message.fees;
      node->counter[1]=message.counter;
      node->sender[1]=sender;
      node->fees[0]=0;
      node->counter[0]=0;
      node->sender[0]=0;
    }
    else{
      node->alice=true;
      node->bob=false;
      node->fees[0]=message.fees;
      node->counter[0]=message.counter;
      node->sender[0]=sender;
      node->fees[1]=0;
      node->counter[1]=0;
      node->sender[1]=0;

    }
    return(node);
  }


struct PHEROMONE* balanced_insert_SEED(struct PHEROMONE_MESSAGE message, int sender, struct PHEROMONE* node) /*AVL-tree insertion: insert a new node labeled by "seed" in the tree with root "node"*/
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(message,sender));
    int seed=message.seed;
    if (seed < node->seed)
        node->left_child  = balanced_insert_SEED(message, sender, node->left_child);
    else if (seed > node->seed)
        node->right_child = balanced_insert_SEED(message, sender, node->right_child);
    else{ // Equal keys are not allowed in BST
        printf("\nSeed was already there\n");
        return node;
    }
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left_child),
                           height(node->right_child));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && seed < node->left_child->seed)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && seed > node->right_child->seed)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && seed > node->left_child->seed)
    {
        node->left_child =  leftRotate(node->left_child);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && seed < node->right_child->seed)
    {
        node->right_child = rightRotate(node->right_child);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

void clear_tree(struct PHEROMONE *tree){
  if(tree==NULL){
    return;
  }
  else{
    clear_tree(tree->left_child);
    clear_tree(tree->right_child);
    free(tree);
  }
}
