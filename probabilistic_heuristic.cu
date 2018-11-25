#include "probabilistic_heuristic.h"
#include "float.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#define GET_LEFT(x)(x<=0?RES_COL-1:x-1)
#define GET_RIGHT(x)(x>=RES_COL-1?0:x+1)
#define GET_ABOVE(x)(x<=0?RES_ROW-1:x-1)
#define GET_BELOW(x)(x>=RES_ROW-1?0:x+1)

float map[RES_COL*RES_ROW*2];
float *device_map;
size_t pitch;


extern "C" void init(){
  cudaMallocPitch(&device_map,&pitch,RES_COL*sizeof(float),RES_ROW*2);
}

__global__ void calculate_cost(float *device_map ,size_t pitch, int food_x, int food_y,int *snake,int length,float snake_length){
  int thread_x=blockIdx.x*blockDim.x+threadIdx.x;
  int thread_y=blockIdx.y*blockDim.y+threadIdx.y;
  if(thread_x < RES_COL && thread_y< RES_ROW){
    float *cost=(float*)(((char*)device_map)+thread_y*pitch); 
    cost[thread_x]= (fminf(fabsf(food_x-thread_x),RES_COL-fabsf(food_x-thread_x))+fminf(fabsf(food_y-thread_y),RES_ROW-fabsf(food_y-thread_y)));
    cost=(float*)(((char*)device_map)+thread_y*pitch+RES_ROW*pitch);
    float x1,y1,x2,y2;
    float op=0,d1,d2,d3,x=(float)thread_x,y=(float)thread_y;
    for(int i=0;i<length;i+=2){
      x1=(float)snake[i];
      y1=(float)snake[i+1];
      x2=(float)snake[i+2];
      y2=(float)snake[i+3];
      d1=powf(x1-x2,2)+powf(y1-y2,2);
      d2=((x-x1)*(x2-x1)+(y-y1)*(y2-y1))/d1;
      d3=fmaxf(.0f,fminf(1.0f,d2));
      d2=sqrtf(powf(x-(x1+d3*(x2-x1)),2)+powf(y-(y1+d3*(y2-y1)),2));
      if(d2==0){
        cost[thread_x]=-1.0f;
        return;
      }
      op+=d2;
    }
    cost[thread_x]=op/snake_length;
  }
}

extern "C" void update_map(float snake_length){
  dim3 threadperblock(16,12);
  dim3 blockpergrid(50,50);
  RenderingLines *temp=head;
  int *snake_h=NULL,*snake_d=NULL,length=1;
  while(temp!=NULL){
    length++;
    temp=temp->next;
  }
  cudaMalloc((void**)&snake_d,sizeof(int)*length*2);
  snake_h=(int*)malloc(sizeof(int)*length*2);
  preprocess(snake_h,length);
  cudaMemcpy((void*)snake_d,(void*)snake_h,sizeof(int)*length*2,cudaMemcpyHostToDevice);
  printf("snake Length=%lf\n",snake_length);
  calculate_cost<<<blockpergrid,threadperblock>>>(device_map,pitch ,f->x1,f->y1,snake_d,length,snake_length);
  cudaDeviceSynchronize();


  cudaMemcpy2D(map,RES_COL*sizeof(float),device_map,pitch,RES_COL*sizeof(float),RES_ROW*2,cudaMemcpyDeviceToHost);
  
  for(int j=head->attr->y1>10?head->attr->y1-10:0;j<RES_ROW && j<head->attr->y1+10;j++){
    for(int i=head->attr->x1>10?head->attr->x1-10:0;i<RES_COL && i<head->attr->x1+10;i++){
      printf("%+.4lf ",((map+((int)RES_COL*j)+(int)RES_ROW*(int)RES_COL)[i]));  
    }
    printf("\n");
  }
  cudaFree(snake_d);
  free(snake_h);
  snake_h=NULL;

}

extern "C" float get_cost(int x,int y){
  return ((map+(RES_COL*y))[x]);
}
extern "C" float get_prob(int x,int y){
  return ((map+(RES_COL*y)+RES_ROW*RES_COL)[x]);
}
extern "C" float calculate_prob(int x,int y,Command c){
  int X,Y;
  float l[5];
  l[0]=get_prob(x,y);
  if(l[0]==-1.0f){
    return 9999.0f;
  }
  X=x<=0?RES_COL-1:x-1;
  l[1]=get_prob(X,y);
  X=x>=RES_COL-1?0:x+1;
  l[2]=get_prob(X,y);
  Y=y<=0?RES_ROW-1:y-1;
  l[3]=get_prob(x,Y);
  Y=y>=RES_ROW-1?0:y+1;
  l[4]=get_prob(x,Y);
  for(int i=1;i<5;i++){
    if(l[i]==-1.0f){
      l[i]=1;
    }
  }
  switch(c){
    case LEFT:
    l[2]=0;
    break;
    case RIGHT:
    l[1]=0;
    break;
    case UP:
    l[4]=0;
    break;
    case DOWN:
    l[3]=0;
    break;
  }
  float sum=0;
  for(int i=0;i<5;i++){
    sum+=l[i];
  }
  sum=sum/4.0f;

  return sum; 
}
extern "C" Command next_command(){
  int x=head->attr->x1;
  int y=head->attr->y1;
  float cost[4],normalizer=0;
  int index=0;
  Command next=LEFT;
  cost[0] = get_cost(GET_LEFT(x), y);
  cost[1] = get_cost(x, GET_ABOVE(y));
  cost[2] = get_cost(GET_RIGHT(x), y);
  cost[3] = get_cost(x, GET_BELOW(y));
  index=0;
  for (int i = 0; i < 4; i++) {
    if(cost[i]<cost[index]){
      index=i;
    }
  }
  normalizer=cost[index];
  for (int i = 0; i < 4; i++) {
    cost[i] = cost[i]/normalizer;
  }

  index=0;
  cost[0] *= calculate_prob(GET_LEFT(x), y, LEFT);
  if (cost[0] < cost[index]) {
    index = 0;
    next = LEFT;
  }
  printf("\nLEFT COST=%lf PROB=%lf MINCOST=%lf",cost[0],get_prob(GET_LEFT(x),y),cost[index]);  

  cost[1] *= calculate_prob(x, GET_ABOVE(y), UP);
  if (cost[1] < cost[index]) {
    index = 1;
    next = UP;
  }
  printf("\nUP COST=%lf PROB=%lf MINCOST=%lf",cost[1],get_prob(x, GET_ABOVE(y)),cost[index]);

  cost[2] *= calculate_prob(GET_RIGHT(x), y, RIGHT);
  if (cost[2] < cost[index]) {
    index = 2;
    next = RIGHT;
  }
  printf("\nRIGHT COST=%lf PROB=%lf MINCOST=%lf",cost[2],get_prob(GET_RIGHT(x), y),cost[index]);

 
  cost[3] *= calculate_prob(x, GET_BELOW(y), DOWN);
  if (cost[3] < cost[index]) {
    index = 3;
    next = DOWN;
  }
  printf("\nDOWN COST=%lf PROB=%lf MINCOST=%lf\n",cost[3],get_prob(x, GET_BELOW(y)),cost[index]);
  return next;
}

extern "C" void preprocess(int *array,int length){
  RenderingLines *temp=head;
  array[0]=temp->attr->x1;
  array[1]=temp->attr->y1;
  array[2]=temp->attr->x2;
  array[3]=temp->attr->y2;
  temp=temp->next;
  int i=2;
  while(temp!=NULL){
    array[i]=temp->attr->x1;
    array[i+1]=temp->attr->y1;
    i+=2;
    temp=temp->next;
  }
  array[(length*2)-2]=last->attr->x2;
  array[(length*2)-1]=last->attr->y2;
  }