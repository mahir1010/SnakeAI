#ifndef GLOBAL_OPTIMUM_H
#define GLOBAL_OPTIMUM_H

#include "export.h"
#include "cuda.h"
#include "cuda_runtime.h"

extern int map[RES_ROW*RES_COL];
extern int *device_map;
extern size_t pitch;
#if __cplusplus
extern "C"{
#endif
void init();
void update_map(int ,int );
#if __cplusplus
}
#endif
__global__ void calculate_cost(int*,size_t, int ,int );

#endif