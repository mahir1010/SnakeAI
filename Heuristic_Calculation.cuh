#ifndef PROB_HEURISTIC_CALCULATION_CUH
#define PROB_HEURISTIC_CALCULATION_CUH

#ifndef SEARCH_DEPTH
#define SEARCH_DEPTH 3
#endif

#include "export.h"
#include "math.h"
#include "cuda.h"
#include "cuda_runtime.h"
__global__ void d_calculate_distance(float ,float ,int *,int ,float *);
__global__ void calculate_cost(float*,size_t, int ,int,int*,int ,float);

extern const int KERNEL_WIDTH;
extern const dim3 TPB;
extern float map[];
extern float *device_map;
extern size_t pitch;

#ifdef __cplusplus
extern "C"{
#endif

void preprocess(int *,int);
void update_map(float);
void init();

#ifdef __cplusplus
}
#endif

#endif