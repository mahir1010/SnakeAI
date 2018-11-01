#include "global_optimum.h"
#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
int map[RES_COL*RES_ROW];
int *device_map;
size_t pitch;

extern "C" void init(){
  cudaMallocPitch(&device_map,&pitch,RES_COL*sizeof(int),RES_ROW);
}

__global__ void calculate_cost(int *device_map ,size_t pitch, int x, int y){
  int thread_x=blockIdx.x*blockDim.x+threadIdx.x;
  int thread_y=blockIdx.y*blockDim.y+threadIdx.y;
  if(thread_x < RES_COL && thread_y< RES_ROW){
    int *cost=(int*)((char*)device_map+thread_y*pitch); 
    cost[thread_x]= (int)(fminf(fabsf(x-thread_x),RES_COL-fabsf(x-thread_x))+fminf(fabsf(y-thread_y),RES_ROW-fabsf(y-thread_y)));
  }
}

extern "C" void update_map(int target_X, int target_y){
  dim3 threadperblock(16,12);
  dim3 blockpergrid(50,50);
  calculate_cost<<<blockpergrid,threadperblock>>>(device_map,pitch ,target_X,target_y);
  cudaDeviceSynchronize();
  cudaMemcpy2D(map,RES_COL*sizeof(int),device_map,pitch,RES_COL*sizeof(int),RES_ROW,cudaMemcpyDeviceToHost);
}
