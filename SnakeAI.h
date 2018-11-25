#ifndef SNAKEAI_H
#define SNAKEAI_H

#include "stdlib.h"
#include "math.h"

#ifndef PROB_HEURISTIC
#include "greedy_local_optimum.h"
#else
#include "probabilistic_heuristic.h"
#endif
#endif