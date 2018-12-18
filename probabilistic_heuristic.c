#include "probabilistic_heuristic.h"
#include "stdlib.h"
#include "stdio.h"
#include "float.h"



float get_cost(int x,int y){
  return (map+((SEARCH_DEPTH+y)*KERNEL_WIDTH))[SEARCH_DEPTH+x];
}

float get_prob(int x,int y){
  float prob=(map+(KERNEL_WIDTH*(KERNEL_WIDTH+SEARCH_DEPTH+y)))[SEARCH_DEPTH+x];
  return prob==-1.0f?-1.0f:1.0f-prob;
}

float calculate_prob(int x,int y,Command c){

  return 1.0f; 
}

void next_command(Command_List **command_head){
  float cost[4];
  cost[0]=get_cost(-1,0); //* get_prob(-1,0);
  cost[1]=get_cost(0,-1); //* get_prob(0,-1);
  cost[2]=get_cost(1,0); //* get_prob(1,0);
  cost[3]=get_cost(0,1);//* get_prob(0,1);
    int key=0;
    for(int i=0;i<4;i++){
     // printf("Cost[%d] before = %lf\n",i,cost[i]);
      //printf("Cost[%d] after = %lf\n",i,cost[i]);
      //printf("last command subtraction %lf\n",fabs(i-currentCommand));
      if(cost[i]<cost[key] && fabs(i-currentCommand)!=2.0f){
        key=i;
      }
      //printf("\n Min Cost at %d , value %lf\n\n",key,cost[key]);
    }
    insert_command(command_head,(Command)key);
    return;
}
