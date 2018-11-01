#ifndef SNAKEAI_H
#define SNAKEAI_H

#include "stdlib.h"
#include "math.h"

#ifndef GLOBAL_OPTIMUM
#include "greedy_local_optimum.h"
#else
#include "global_optimum.h"
#endif
#endif