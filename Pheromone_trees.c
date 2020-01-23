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
#include "tree_functions.h"


extern struct PHEROMONE *PheromoneTree;

int *seed_array;

void generate_tree(int k){
  seed_array=malloc(k*sizeof(int));
  int i=0;
  int new_SEED;
  for(i=0;i<k;i++){
    new_SEED=rand();
    while(search_SEED(new_SEED,PheromoneTree)!=NULL){
      new_SEED=rand();
    }
    *(seed_array+i)=new_SEED;
    bool origin=rand()%2;
    int counter=rand();
    int fees=rand();
    int amount=rand();
    int sender=rand()%100;
    time_t timestamp=time(NULL);
    struct PHEROMONE_MESSAGE message={origin, new_SEED, counter, fees, amount, timestamp};
    PheromoneTree=balanced_insert_SEED(message,sender, PheromoneTree);
  }
}




int main(){
  srand(time(NULL));
  int SEED_number;
  clock_t start,end;
	double execution_time;
  int request;

  MEMPOOL_GENERATION: printf("\nHow many seeds do  you want to introduce?");
  scanf("%d",&SEED_number);
  srand(time(NULL));
  start=clock();
  generate_tree(SEED_number);
  end=clock();
  execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
  printf("\nMempool was generated in %f seconds\n",execution_time);


  while(true){
    printf("\nWhat do you want to do?\n 1.Insert seed \n 2. Search tree.\n 3. Search an already existing seed in tree\n 4. Record insertion time \n 5. Record search time\n 6. Record erasing time\n 7. Erase mempool");
    scanf("%d",&request);

    if(request==1){
      printf("\nEnter a new seed:");
      int new_SEED;
      scanf("%d",&new_SEED);
      if(new_SEED==0){
        break;
      }
      bool origin=rand()%2;
      int counter=rand();
      int fees=rand();
      int amount=rand();
      time_t timestamp=time(NULL);
      int sender=rand()%100;
      struct PHEROMONE_MESSAGE message={origin, new_SEED, counter, fees, amount, timestamp};
      start=clock();
      PheromoneTree=balanced_insert_SEED(message, sender, PheromoneTree);
      end=clock();
      execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
      printf("\nSeed has been inserted in %f seconds\n",execution_time);
    }

    else if(request==2){
      printf("\nEnter seed to search:");
      int new_SEED;
      scanf("%d",&new_SEED);
      struct PHEROMONE *lookup;
      start=clock();
      lookup=search_SEED(new_SEED,PheromoneTree);
      end=clock();
      execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
      if(lookup==NULL){
        printf("\n Seed not in the tree\n");
      }
      else{
        printf("\n Seed is in the tree\n");
      }
      printf("\nExecution time: %f seconds\n",execution_time);
    }
    else if(request==3){
      int index=rand()%SEED_number;
      int new_SEED=*(seed_array+index);
      struct PHEROMONE *lookup;
      start=clock();
      lookup=search_SEED(new_SEED,PheromoneTree);
      end=clock();
      execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
      if(lookup==NULL){
        printf("\n Seed not in the tree\n");
      }
      else{
        printf("\n Seed is in the tree\n");
      }
      printf("\nExecution time: %f seconds\n",execution_time);
    }
    else if(request==4||request==5){
      PheromoneTree=NULL;
      printf("\nChoose mempool size:");
      scanf("%d",&SEED_number);
      printf("\nChoose sample size:");
      int sample_size;
      scanf("%d",&sample_size);
      int counter=0;
      int new_SEED;
      FILE *stats;

      if(request==4){
        stats=fopen("insertion_time.R","r+");
        fseek(stats,0,SEEK_END);
  			fprintf(stats,"\nv%d<-c(",SEED_number);
        printf("\nRecording insertion time for mempool with %d seeds, sample size %d\n",SEED_number,sample_size);
        for(counter=0;counter<sample_size;counter++){
          generate_tree(SEED_number);
          usleep(100000);
          new_SEED=rand();
          while(search_SEED(new_SEED,PheromoneTree)!=NULL){
            usleep(100000);
            new_SEED=rand();
          }
          bool origin=rand()%2;
          int counter=rand();
          int fees=rand();
          int amount=rand();
          time_t timestamp=time(NULL);
          int sender=rand()%100;
          struct PHEROMONE_MESSAGE message={origin, new_SEED, counter, fees, amount, timestamp};

          start=clock();
          PheromoneTree=balanced_insert_SEED(message,sender, PheromoneTree);
          end=clock();
          execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
          fprintf(stats,"%f,",execution_time);
          clear_tree(PheromoneTree);
          free(seed_array);
          PheromoneTree=NULL;
        }
        fseek(stats,-1,SEEK_CUR);
  			fprintf(stats,")\n");
  			fprintf(stats,"m%d<-mean(v%d)\n",SEED_number,SEED_number);
  			fclose(stats);
      }
      if(request==5){
        stats=fopen("lookup_time.R","r+");
        fseek(stats,0,SEEK_END);
  			fprintf(stats,"\nv%d<-c(",SEED_number);
        printf("\nRecording look-up time for mempool with %d seeds, sample size %d\n",SEED_number,sample_size);
        for(counter=0;counter<sample_size;counter++){
          generate_tree(SEED_number);
          usleep(100000);
          new_SEED=rand();
          struct PHEROMONE *lookup;
          start=clock();
          lookup=search_SEED(new_SEED,PheromoneTree);
          end=clock();
          if(lookup){
            counter--;
            continue;
          }
          execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
          fprintf(stats,"%f,",execution_time);
          clear_tree(PheromoneTree);
          free(seed_array);
          PheromoneTree=NULL;
        }
        fseek(stats,-1,SEEK_CUR);
  			fprintf(stats,")\n");
  			fprintf(stats,"m%d<-mean(v%d)\n",SEED_number,SEED_number);
  			fclose(stats);
      }
    }

    else if(request==6){
      clear_tree(PheromoneTree);
      PheromoneTree=NULL;
      printf("\nChoose mempool size:");
      scanf("%d",&SEED_number);
      printf("\nChoose sample size:");
      int sample_size;
      scanf("%d",&sample_size);
      int counter=0;
      FILE *stats;
      stats=fopen("cleaning_time.R","r+");
      fseek(stats,0,SEEK_END);
      fprintf(stats,"\nv%d<-c(",SEED_number);
      printf("\nRecording cleaning time for mempool with %d seeds, sample size %d\n",SEED_number,sample_size);
      for(counter=0;counter<sample_size;counter++){
        generate_tree(SEED_number);
        start=clock();
        clear_tree(PheromoneTree);
        end=clock();
        PheromoneTree=NULL;
        execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
        fprintf(stats,"%f,",execution_time);
        free(seed_array);
      }
      fseek(stats,-1,SEEK_CUR);
      fprintf(stats,")\n");
      fprintf(stats,"m%d<-mean(v%d)\n",SEED_number,SEED_number);
      fclose(stats);
    }
    else if(request==7){
      start=clock();
      clear_tree(PheromoneTree);
      free(seed_array);
      PheromoneTree=NULL;
      end=clock();
      execution_time=((double) (end - start)) / CLOCKS_PER_SEC;
      printf("\nCleaning time was %f seconds\n",execution_time);
      goto MEMPOOL_GENERATION;
    }
    else if(request==0){
      break;
    }
    else{
      printf("\nInvalid request\n");
      continue;
    }
  }
  return 0;
}
