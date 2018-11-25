#ifndef PROBABILISTIC_HEURISTIC_H
#define PROBABILISTIC_HEURISTIC_H

#include "export.h"
#include "cuda.h"
#include "cuda_runtime.h"

extern float map[RES_ROW*RES_COL*2];
extern float *device_map;
extern size_t pitch;
#if __cplusplus
extern "C"{
#endif
void init();
void update_map(float);
void calculate_path();
float calculate_prob(int,int,Command);
float get_cost(int,int);
float get_prob(int,int);
void preprocess(int *,int);
Command next_command();
#if __cplusplus
}
#endif
__global__ void calculate_cost(float*,size_t, int ,int,int*,int ,float);
#endif