#include "Heuristic_Calculation.cuh"
#include "stdio.h"
#include "math.h"
#include "float.h"
const int KERNEL_WIDTH=SEARCH_DEPTH*2+1;
const dim3 TPB(KERNEL_WIDTH,KERNEL_WIDTH,1);
float map[KERNEL_WIDTH*KERNEL_WIDTH];
float *device_map;
size_t pitch;

void init(){
  cudaMallocPitch(&device_map,&pitch,KERNEL_WIDTH*sizeof(float),KERNEL_WIDTH);
}

__global__ void d_calculate_distance(float x,float y,int *snake,int length,float *op){
  extern __shared__ float coordinates[];
  int index=threadIdx.x*4;
  float d2,d3;
  coordinates[index]=(float)snake[index];
  coordinates[index+1]=(float)snake[index+1];
  coordinates[index+2]=(float)snake[index+2];
  coordinates[index+3]=(float)snake[index+3];
  d2=((x-coordinates[index])*(coordinates[index+2]-coordinates[index])+(y-coordinates[index+1])*(coordinates[index+3]-coordinates[index+1]))/(powf(coordinates[index]-coordinates[index+2],2)+powf(coordinates[index+1]-coordinates[index+3],2));
  d3=fmaxf(.0f,fminf(1.0f,d2));
  d2=sqrtf(powf(x-(coordinates[index]+d3*(coordinates[index+2]-coordinates[index])),2)+powf(y-(coordinates[index+1]+d3*(coordinates[index+3]-coordinates[index+1])),2));
  if(d2<1.0f){
    *op=-1.0f;
  }
  __syncthreads();
  if(*op==-1.0f){
    return;
  }
  coordinates[length*4]=.0f;
  atomicAdd(&coordinates[length*4],d2);
  __syncthreads();
  if(threadIdx.x==0){
    *op=coordinates[length*4]/(RES_ROW*RES_COL);
  }
}
__global__ void calculate_cost(float *device_map ,size_t pitch, int food_x, int food_y,int *snake,int length,float snake_length){
  int X=GET_X(snake[0],((int)(threadIdx.x-SEARCH_DEPTH)));
  int Y=GET_Y(snake[1],((int)(threadIdx.y-SEARCH_DEPTH)));
  float x1,y1,x2,y2;
  float *cost=(float*)(((char*)device_map)+threadIdx.y*pitch); 
  cost[threadIdx.x]= fminf(fabsf(food_x-X),((float)RES_COL)-fabsf(food_x-X)) + fminf(fabsf(food_y-Y),((float)RES_ROW)-fabsf(food_y-Y));
  float x=(float)X,y=(float)Y;
  /*if(length>8){
    float *opt;
    cudaMalloc((void**)&opt,sizeof(float));
    d_calculate_distance<<<1,length,sizeof(float)*(length*4+1)>>>(x,y,snake,length,opt);
    cudaDeviceSynchronize();
    cost[threadIdx.x]=*opt;
  }else{
    */
    float  op=0,d2,d3;
    for(int i=0;i<length*4;i+=4){
      x1=(float)snake[i];
      y1=(float)snake[i+1];
      x2=(float)snake[i+2];
      y2=(float)snake[i+3];
      d2=((x-x1)*(x2-x1)+(y-y1)*(y2-y1))/(powf(x1-x2,2)+powf(y1-y2,2));
      d3=fmaxf(.0f,fminf(1.0f,d2));
      d2=sqrtf(powf(x-(x1+d3*(x2-x1)),2)+powf(y-(y1+d3*(y2-y1)),2));
      if(d2<1.0f){
        cost[threadIdx.x]=FLT_MAX;
        return;
      }
      op+=d2;
    }
    cost[threadIdx.x]=cost[threadIdx.x]*(1.0f-op/(RES_COL*RES_ROW));
  //}
}

void update_map(float snake_length){
  RenderingLines *temp=head;
  int *snake_h=NULL,*snake_d=NULL,length=0;
  while(temp!=NULL){
    temp=temp->next;
    length++;
  }
  cudaMalloc((void**)&snake_d,sizeof(int)*length*4);
  snake_h=(int*)malloc(sizeof(int)*length*4);
  preprocess(snake_h,length);
  cudaMemcpy((void*)snake_d,(void*)snake_h,sizeof(int)*length*4,cudaMemcpyHostToDevice);
  calculate_cost<<<1,TPB>>>(device_map,pitch ,f->x1,f->y1,snake_d,length,snake_length);
  cudaDeviceSynchronize();
  cudaMemcpy2D((void*)map,KERNEL_WIDTH*sizeof(float),(void*)device_map,pitch,KERNEL_WIDTH*sizeof(float),KERNEL_WIDTH,cudaMemcpyDeviceToHost);
  /*printf("\n\nIN UPDATE MAP Head at %d %d FOOD at %d %d\n",head->attr->x1,head->attr->y1,f->x1,f->y1);
  printf("\n");
  for(int i=0;i<KERNEL_WIDTH;i++){
    int slice=KERNEL_WIDTH*(KERNEL_WIDTH+i);
    for(int j=0;j<KERNEL_WIDTH;j++){
      printf("%+lf ",(map+slice)[j]);
    }
    printf("\n");
  }
  printf("\n\n");
  */
  cudaFree(snake_d);
  free(snake_h);
  snake_h=NULL;

}

void preprocess(int *array,int length){
  RenderingLines *temp=head;
  int i=0;
  while(temp!=NULL){
    array[i]=temp->attr->x1;
    array[i+1]=temp->attr->y1;
    array[i+2]=temp->attr->x2;
    array[i+3]=temp->attr->y2;
    i+=4;
    temp=temp->next;
  }
 /* printf("Points ");
  for(int i=0;i<length*4;i++){
    printf("%d ",array[i]);
    if((i+1)%4==0){
      printf("\n");
    }
  }
  printf("\n");
  */
}